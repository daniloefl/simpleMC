///
/// Main generator of events, outputting a HepMC file
/// with the initial and final state configurations.
///
/// @author Danilo Ferreira de Lima <daniloefl@gmail.com>
///

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <map>

#include "TLorentzVector.h"
#include "TRandom3.h"

#include "Selectors.h"
#include "Process.h"
#include "BhabhaProcess.h"
#include "ProcessComp.h"
#include "Helper.h"

#include "Unweight.h"

#include <unistd.h>
#include <getopt.h>

#include <algorithm>
#include <cctype>

using namespace std;

void showHelp() {
  std::cout << "This program simulates scattering of electron-positron -> Z -> e+e-." << std::endl
            << "All four-momenta are written, one per line of the output text file." << std::endl;
  std::cout << std::endl;
  std::cout << "Usage:" << std::endl;
  std::cout << "generate [options]" << std::endl
            << "where [options] are one of:" << std::endl
            << std::endl
            << "   -h  | --help                Show this help message." << std::endl
            << "   -f  | --file [FILE]         Write results in file [FILE]." << std::endl
            << "   -e  | --events [N]          Produce [N] events before unweighting." << std::endl
            << "   -E  | --energy [VALUE]      Simulate electron-positron collisions with center-of-mass energy given by [VALUE] GeV." << std::endl
            << "   -u  | --unweight            If this flag is sent, apply unweighting." << std::endl
            << "   -p  | --process [NAME]      'Bhabha' for the electron-positron -> photon -> electron-positron QED scattering and 'EWK' to also include Z boson mediators." << std::endl
            << "   -s  | --scanCoM [SIGMA]     Vary centre-of-mass according to a uniform distribution of width [SIGMA]." << std::endl
            << std::endl;
}

int main(int argc, char *argv[]) {

  std::string outfile = "events.txt";
  int maxEvents = (int) 1e6;
  double Eb = 45; // beam energy [GeV]
  double randomCoM = 0; // CoM std. deviation
  std::string process = "ewk";

  // if this is done, the weights are set to 1
  // this way statistical fluctuations are minimised
  // very inefficient, but improves statistical uncertainty
  bool doUnweighting = false;

  int c;
  int digit_optind = 0;

  while (true) {
    int this_option_optind = optind ? optind : 1;
    int option_index = 0;
    static struct option long_options[] = {
            {"help",      no_argument,       0, 'h'},
            {"file",      required_argument, 0, 'f'},
            {"events",    required_argument, 0, 'e'},
            {"energy",    required_argument, 0, 'E'},
            {"unweigh",   no_argument,       0, 'u'},
            {"scanCoM",   required_argument, 0, 's'},
            {"process",   required_argument, 0, 'p'},
            {0,         0,                 0,  0 }
        };

    c = getopt_long(argc, argv, "f:e:E:us:p:h",
                    long_options, &option_index);
    if (c == -1)
      break;

    switch (c) {
      case 'h':
        showHelp();
        exit(0);
        break;

      case 'f':
        outfile = optarg;
        break;

      case 'e':
        maxEvents = atoi(optarg);
        break;

      case 'E':
        Eb = atof(optarg);
        break;

      case 'u':
        doUnweighting = true;
        break;

      case 's':
        randomCoM = atof(optarg);
        break;

      case 'p':
        process = optarg;
        std::transform(process.begin(), process.end(), process.begin(), ::tolower);
        break;

      case '?':
      default:
        showHelp();
        exit(0);
        break;
    }
  }

  int nEvents = 0;
  int passSelection = 0;

  // beam parameters
  double s = 4*Eb*Eb; // s = center-of-mass energy^2

  std::ofstream ostr(outfile.c_str());
  if( !ostr || !ostr.good() ) {
    std::cout << "Cannot open file " << outfile << std::endl;
    return -1;
  }

  // beam four-momenta
  // e+ e- -> X
  TLorentzVector b1(0, 0,  Eb, Eb);
  TLorentzVector b2(0, 0, -Eb, Eb);

  Process *p = 0;
  if (process == "ewk") {
    p = new ProcessComp;    // calculates results by multiplying out gamma matrices
  } else {
    p = new BhabhaProcess;  // has the cross section distribution already written down
  }
  MySelector sel;       // cuts to apply

  // calculate cross section
  int iterations = 50000;
  double sig_error = 0;
  std::cout << "Full cross section (after cuts in Selector) calculated at CoM energy = " << 2*Eb << " GeV" << std::endl;
  double sigma = p->getSigma(b1, b2, iterations, sig_error, sel);
  std::cout << "Sigma = " << sigma << " +/- " << sig_error << "  (" << 100*sig_error/sigma << "%) femtobarns [fb]" << std::endl;

  Unweight u(*p);
  double maxw = 0;
  if (doUnweighting) {
    maxw = u.getMax(b1, b2, iterations, sel);
    std::cout << "Maximum weight = " << maxw << " (to be used for unweighting)"<< std::endl;
  }

  TRandom3 rand;

  int passUnweight = 0;

  ostr << "#" << std::setw(20) << "sqrt(s)" << std::setw(20) << "alpha_s" << std::setw(20) << "alpha_qed"
       << std::setw(20) << "weight"
       << std::setw(20) << "beam1_px" << std::setw(20) << "beam1_py" << std::setw(20) << "beam1_pz" << std::setw(20) << "beam1_E"
       << std::setw(20) << "beam2_px" << std::setw(20) << "beam2_py" << std::setw(20) << "beam2_pz" << std::setw(20) << "beam2_E"
       << std::setw(20) << "final1_px" << std::setw(20) << "final1_py" << std::setw(20) << "final1_pz" << std::setw(20) << "final1_E"
       << std::setw(20) << "final2_px" << std::setw(20) << "final2_py" << std::setw(20) << "final2_pz" << std::setw(20) << "final2_E" << std::endl;

  std::cout << "In event loop, the CoM energy is varied by +/- 20 GeV around the nominal of " << 2*Eb << std::endl;
  for (nEvents = 0; nEvents < maxEvents; ++nEvents) {
    //double Ebv = Eb + (2*randomDouble()-1)*10;
    double Ebv = Eb;
    if (randomCoM > 0) Ebv += (rand.Uniform(randomCoM)-0.5);

    if (nEvents%10000 == 0)
      std::cout << "Raw events: " << nEvents << ", pass selection: " << passSelection << ", pass unweighting: " << passUnweight << ", this CoM E = " << 2*Ebv << " GeV" << std::endl;

    // energy scale
    double Q = std::sqrt(s);

    TLorentzVector beam1(0,0,-Ebv,Ebv);
    TLorentzVector beam2(0,0,Ebv,Ebv);

    // now do the real work:
    // (only an example now)
    // (should check which process and generate phase space)
    // (for multiple processes, throw random number based on the cross
    //  section and then generate one process)
    std::vector<TLorentzVector> out;
    double mcWeight = 0;
    // check if the cuts in p.select() pass. If they do, then accept
    // the event with weight mcWeight
    if (!p->getOut(beam1, beam2, out, mcWeight, sel))
      continue;
    // now we have the output particles out and the
    // weight for this event, w

    // calculate the number of events used:
    passSelection += 1;

    // if we should do unweighting, then compare the weight with the maximum weight
    if (doUnweighting) {
      if (!u.accept(mcWeight)) {
        continue;
      }
      // event was accepted, so set the weight to 1
      // the different probabilities of the process
      // are now taken into account in the accept/reject phase of
      // the unweighting
      mcWeight = 1.0;
    }

    ostr << std::setw(20) << Q << std::setw(20) << a_s << std::setw(20) << alpha_QED(s)
       << std::setw(20) << mcWeight
       << std::setw(20) << beam1.Px() << std::setw(20) << beam1.Py() << std::setw(20) << beam1.Pz() << std::setw(20) << beam1.E()
       << std::setw(20) << beam2.Px() << std::setw(20) << beam2.Py() << std::setw(20) << beam2.Pz() << std::setw(20) << beam2.E()
       << std::setw(20) << out[0].Px() << std::setw(20) << out[0].Py() << std::setw(20) << out[0].Pz() << std::setw(20) << out[0].E()
       << std::setw(20) << out[1].Px() << std::setw(20) << out[1].Py() << std::setw(20) << out[1].Pz() << std::setw(20) << out[1].E()
       << std::endl;

    passUnweight += 1;

  }
  std::cout << "Raw number of events generated: " << nEvents << " (100%)" << std::endl;
  std::cout << "Pass the kinematical selection: " << passSelection << " (" << 100.0*((double) passSelection)/((double) nEvents) << "%)" << std::endl;
  std::cout << "Pass unweighting procedure:     " << passUnweight << " (" << 100.0*((double) passUnweight)/((double) nEvents) << "%)" << std::endl;
  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << "Selection efficiency:           " << 100.0*((double) passSelection)/((double) nEvents) << "%" << std::endl;
  std::cout << "Unweighting efficiency:         " << 100.0*((double) passUnweight)/((double) passSelection) << "%" << std::endl;

  if (p) delete p;


  return 0;
}

