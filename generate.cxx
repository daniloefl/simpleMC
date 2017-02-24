#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <map>

#include "HepMC/IO_GenEvent.h"
#include "HepMC/GenEvent.h"
#include "HepMC/GenParticle.h"

#include "TLorentzVector.h"

#include "TRandom3.h"

#include "Selectors.h"
#include "Process.h"
#include "BhabhaProcess.h"
#include "ProcessComp.h"
#include "Helper.h"

#include "Unweight.h"

using namespace std;
using namespace HepMC;

int main(int argc, char *argv[]) {

  std::string outfile = "input.hepmc2g";
  int maxEvents = (int) 1e6;

  int nEvents = 0;
  int passSelection = 0;

  // beam parameters
  // should read from user
  //double Eb = 30; // beam energy
  double Eb = 45; // beam energy
  double s = 4*Eb*Eb; // s = center-of-mass energy^2
  int beam1id = -11; // positron
  int beam2id = 11;  // electron

  // if this is done, the weights are set to 1
  // this way statistical fluctuations are minimised
  // very inefficient, but improves statistical uncertainty
  bool doUnweighting = true;

  std::ofstream ostr(outfile.c_str());
  if( !ostr || !ostr.good() ) {
    std::cout << "Cannot open file " << outfile << std::endl;
    return -1;
  }

  // calculate cross section
  GenParticle *b1 = new GenParticle;
  GenParticle *b2 = new GenParticle;
  b1->set_momentum(FourVector(0,0,-Eb,Eb));
  b2->set_momentum(FourVector(0,0, Eb,Eb));
  b1->set_pdg_id(beam1id);
  b1->set_status(0); // CHECK THE CORRECT VALUE FOR INITIAL STATE PARTICLES
  b1->set_pdg_id(beam2id);
  b2->set_status(0); // CHECK

  //BhabhaProcess p; // interaction to calculate: should be sent by user
  ProcessComp p;
  MySelector sel;  // cuts to apply on p_T and eta: should be sent by user

  // calculate cross section
  int iterations = 50000;
  //int iterations = -1;
  double sig_error = 0;
  std::cout << "Full cross section (after cuts in Selector) calculated at CoM energy = " << 2*Eb << " GeV" << std::endl;
  double sigma = p.getSigma(b1, b2, iterations, sig_error, sel);
  std::cout << "Sigma = " << sigma << " +/- " << sig_error << "  (" << 100*sig_error/sigma << "%) femtobarns [fb]" << std::endl;

  Unweight u(p);
  double maxw = 0;
  if (doUnweighting) {
    maxw = u.getMax(b1, b2, iterations, sel);
    std::cout << "Maximum weight = " << maxw << " (to be used for unweighting)"<< std::endl;
  }


  delete b1;
  delete b2;

  int passUnweight = 0;

  HepMC::GenEvent evt;
  std::cout << "In event loop, the CoM energy is varied by +/- 20 GeV around the nominal of " << 2*Eb << std::endl;
  for (nEvents = 0; nEvents < maxEvents; ++nEvents) {
    evt.clear();
    double Ebv = Eb + (2*randomDouble()-1)*10;
    if (nEvents%10000 == 0)
      std::cout << "Raw events: " << nEvents << ", pass selection: " << passSelection << ", pass unweighting: " << passUnweight << ", this CoM E = " << 2*Ebv << " GeV" << std::endl;

    evt.set_signal_process_id(0); // set a some ID for the process
    evt.set_event_number(nEvents);
    evt.set_mpi(1); // multiple parton interactions

    double Q = std::sqrt(s);
    evt.set_event_scale(Q); // set energy scale

    // set scales
    evt.set_alphaQCD(a_s);
    evt.set_alphaQED(alpha_QED(s));

    // set beam particles
    GenParticle *beam1 = new GenParticle;
    GenParticle *beam2 = new GenParticle;
    beam1->set_momentum(FourVector(0,0,-Ebv,Ebv));
    beam2->set_momentum(FourVector(0,0, Ebv,Ebv));
    beam1->set_pdg_id(beam1id);
    beam1->set_status(0); // CHECK
    beam1->set_pdg_id(beam2id);
    beam2->set_status(0); // CHECK
    evt.set_beam_particles(beam1, beam2);

    // now do the real work:
    // (only an example now)
    // (should check which process and generate phase space)
    // (for multiple processes, throw random number based on the cross
    //  section and then generate one process)
    std::vector<GenParticle> out;
    double mcWeight = 0;
    // check if the cuts in p.select() pass. If they do, then accept
    // the event with weight mcWeight
    if (!p.getOut(beam1, beam2, out, mcWeight, sel))
      continue;
    // now we have the output particles out and the
    // weight for this event, w

    // calculate the number of events used:
    passSelection += 1;

    // if we should do unweighting, then compare the weight with the maximum weight
    if (doUnweighting) {
      if (!u.accept(mcWeight)) {
        delete beam1;
        delete beam2;
        continue;
      }
      // event was accepted, so set the weight to 1
      // the different probabilities of the process
      // are now taken into account in the accept/reject phase of
      // the unweighting
      mcWeight = 1.0;
    }

    // add main interaction vertex
    GenVertex *v = new GenVertex;
    // add particles
    v->add_particle_in(beam1);
    v->add_particle_in(beam2);
    v->add_particle_out(new GenParticle(out[0]));
    v->add_particle_out(new GenParticle(out[1]));
    evt.add_vertex(v);
    evt.set_signal_process_vertex(v);
    evt.define_units(HepMC::Units::GEV, HepMC::Units::MM);

    WeightContainer w;
    w.push_back(mcWeight); // MC weight
    w.push_back(0); // unused
    w.push_back(1); // unweighting trials (to be used for normalisation if events are discarded when generated)
    evt.weights() = w; // add weights

    passUnweight += 1;

    // write it in the file
    //ascii_in >> evt;
    try {
      ostr << evt;
    } catch (HepMC::IO_Exception& e) {
      evt.clear();
    }

  }
  std::cout << "Raw number of events generated: " << nEvents << " (100%)" << std::endl;
  std::cout << "Pass the kinematical selection: " << passSelection << " (" << 100.0*((double) passSelection)/((double) nEvents) << "%)" << std::endl;
  std::cout << "Pass unweighting procedure:     " << passUnweight << " (" << 100.0*((double) passUnweight)/((double) nEvents) << "%)" << std::endl;
  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << "Selection efficiency:           " << 100.0*((double) passSelection)/((double) nEvents) << "%" << std::endl;
  std::cout << "Unweighting efficiency:         " << 100.0*((double) passUnweight)/((double) passSelection) << "%" << std::endl;

  return 0;
}

