#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <map>

#include "TLorentzVector.h"

#include "TFile.h"
#include "TTree.h"

#include <unistd.h>
#include <getopt.h>

using namespace std;

int el_n = 0;
std::vector<double> *el_pt = 0;
std::vector<double> *el_m = 0;
std::vector<double> *el_eta = 0;
std::vector<double> *el_phi = 0;
double combinedmass = 0;

double weight = 0;

void showHelp() {
  std::cout << "This program reads a text file with simulations of electron-positron scattering produced by generate and produces a ROOT file with that information for easy plotting." << std::endl;
  std::cout << std::endl;
  std::cout << "Usage:" << std::endl;
  std::cout << "Analysis [options]" << std::endl
            << "where [options] are one of:" << std::endl
            << std::endl
            << "   -h  | --help             Show this help message." << std::endl
            << "   -f  | --file [FILE]      Input in file [FILE]." << std::endl
            << "   -o  | --output [FILE]    Output ROOT file to be written in [FILE]." << std::endl
            << "   -e  | --events [N]       Read up to [N] events." << std::endl
            << std::endl;
}

int main(int argc, char *argv[]) {

  std::string file = "events.txt";
  std::string outFileName = "out.root";
  int maxEvents = 500000;

  int c;
  int digit_optind = 0;

  while (true) {
    int this_option_optind = optind ? optind : 1;
    int option_index = 0;
    static struct option long_options[] = {
            {"help",    no_argument,       0, 'h'},
            {"file",    required_argument, 0, 'f'},
            {"output",  required_argument, 0, 'o'},
            {"events",  required_argument, 0, 'e'},
            {0,         0,                 0,  0 }
        };

    c = getopt_long(argc, argv, "f:o:e:h",
                    long_options, &option_index);
    if (c == -1)
      break;

    switch (c) {
      case 'h':
        showHelp();
        exit(0);
        break;

      case 'f':
        file = optarg;
        break;

      case 'e':
        maxEvents = atoi(optarg);
        break;

      case 'o':
        outFileName = optarg;
        break;

      case '?':
      default:
        showHelp();
        exit(0);
        break;
    }
  }

  // for ROOT analysis
  TFile *outFile = new TFile(outFileName.c_str(), "RECREATE");
  TTree *tree = new TTree("analysis", "");

  tree->Branch("el_n", &el_n);
  tree->Branch("el_pt", &el_pt);
  tree->Branch("el_m", &el_m);
  tree->Branch("el_eta", &el_eta);
  tree->Branch("el_phi", &el_phi);
  tree->Branch("combinedmass", &combinedmass);

  tree->Branch("weight", &weight);

  int nEvents = 0;
  int passSelection = 0;

  std::ifstream istr(file.c_str());
  if( !istr || !istr.good() ) {
    std::cout << "Cannot open file " << file << std::endl;
    return -1;
  }
  std::string line;
  while (std::getline(istr, line)) {
    if (line[0] == '#') continue;
    if (nEvents >= maxEvents) break;

    weight = 1;
    el_n = 0;
    el_pt->clear();
    el_m->clear();
    el_eta->clear();
    el_phi->clear();
    combinedmass = 0;

    if (nEvents%1000 == 0)
      std::cout << "Event " << nEvents << ", passed # = " << passSelection << endl;

    nEvents++;

    std::stringstream sstr(line);

    TLorentzVector combo(0,0,0,0);

    double Q;
    double a_s;
    double alpha_QED;
    double px, py, pz, E;

    sstr >> Q >> a_s >> alpha_QED;

    sstr >> weight;

    // beam 1
    sstr >> px >> py >> pz >> E;
    // beam 2
    sstr >> px >> py >> pz >> E;

    TLorentzVector mom;

    // lepton 1
    sstr >> px >> py >> pz >> E;
    mom.SetPxPyPzE(px, py, pz, E);
    el_n++;
    el_pt->push_back(mom.Perp());
    el_eta->push_back(mom.Eta());
    el_phi->push_back(mom.Phi());
    el_m->push_back(mom.M());
    combo += mom;

    // lepton 2
    sstr >> px >> py >> pz >> E;
    mom.SetPxPyPzE(px, py, pz, E);
    el_n++;
    el_pt->push_back(mom.Perp());
    el_eta->push_back(mom.Eta());
    el_phi->push_back(mom.Phi());
    el_m->push_back(mom.M());

    combo += mom;

    combinedmass = combo.M();

    // calculate the number of events used:
    passSelection += 1;

    tree->Fill();
  }
  std::cout << "Number of events that passed selection = " << passSelection << " of a total of " << nEvents << std::endl;

  outFile->Write();
  outFile->Close();
  return 0;
}

