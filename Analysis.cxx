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

#include "TFile.h"
#include "TTree.h"

#include "TH1D.h"
#include "TCanvas.h"

#include "TRandom3.h"

using namespace std;
using namespace HepMC;

// not used
const int allBhadrons[] = {
                           511,513,515,521,523,525,531,533,535,541,543,545,551,553,555,
                           5101,5103,5112,5114,5122,5132,5142,5201,5203,5212,5214,5222,5224,
                           5232,5242,5301,5303,5312,5314,5322,5324,5332,5334,5342,5401,5403,
                           5412,5414,5422,5424,5432,5434,5442,5444,5503,5512,5514,5522,5524,
                           5532,5534,5542,5544,5554,10511,10513,10521,10523,10531,10533,10541,
                           10543,10551,10553,20513,20523,20533,20543,20553,100441,100443,100553};

bool IsNeutrino(int pid) {
  if (abs(pid) == 12 || abs(pid) == 14 || abs(pid) == 16)
    return true;
  return false;
}

bool IsGamma(int pid) {
  if (abs(pid) == 22)
    return true;
  return false;
}

bool IsNeutron(int pid) {
  if (abs(pid) == 2112)
    return true;
  return false;
}

bool IsK0(int pid) {
  if (abs(pid) == 130 || abs(pid) == 310 || abs(pid) == 311 )
    return true;
  return false;
}

bool IsPi0(int pid) {
  if (abs(pid) == 111 || abs(pid) == 113 || abs(pid) == 221 )
    return true;
  return false;
}

bool IsElectron(int pid) {
  if (abs(pid) == 11)
    return true;
  return false;
}

bool IsMuon(int pid) {
  if (abs(pid) == 13)
    return true;
  return false;
}

bool IsCharged(int pid) { 
  if (IsNeutrino(pid) || IsGamma(pid) || IsNeutron(pid) ||
      IsK0(pid) || IsPi0(pid) || abs(pid)== 94 ||
      abs(pid) == 1000039 || abs(pid) ==1000022 || abs(pid) ==39 ||
      abs(pid) == 5100022) {
    return false;
  }
  return true;
}

TRandom3 randgen;

int el_n = 0;
std::vector<double> *el_pt = 0;
std::vector<double> *el_m = 0;
std::vector<double> *el_eta = 0;
std::vector<double> *el_phi = 0;
double combinedmass = 0;

double weight = 0;
double trials = 0;

bool isBhadron(int pid) {
  if ( (pid >= 511 && pid <= 545) || (pid >= 10511 && pid <= 10545) || (pid >= 20511 && pid <= 20545) || (pid >= 5112 && pid <= 5554)) {  // check if it's a B-hadron
    return true;
  }
  return false;
}
bool isChadron(int pid) {
  if ( (pid >= 400 && pid < 500 && pid != 443) || (pid >= 4000 && pid < 5000) || (pid >= 10411 && pid <= 10455) || (pid >= 20411 && pid <= 20445)) {  // check if it's a C-hadron
    return true;
  }
  return false;
}

int main(int argc, char *argv[]) {

  std::string file = "input.hepmc2g";
  std::string outFileName = "out.root";
  int maxEvents = 500000;

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
  tree->Branch("trials", &trials);

  int nEvents = 0;
  int passSelection = 0;

  std::ifstream istr(file.c_str());
  if( !istr || !istr.good() ) {
    std::cout << "Cannot open file " << file << std::endl;
    return -1;
  }
  HepMC::set_input_units( istr, HepMC::Units::GEV, HepMC::Units::MM );
  HepMC::GenEvent evt;
  while (istr) {
    try {
      evt.clear();
      istr >> evt;
    } catch (HepMC::IO_Exception& e) {
      evt.clear();
    }
    if (!evt.is_valid()) {
      std::cout << "Invalid event" << std::endl;
      continue;
    }
    if (nEvents >= maxEvents) break;

    weight = 1;
    trials = 1;
    el_n = 0;
    el_pt->clear();
    el_m->clear();
    el_eta->clear();
    el_phi->clear();
    combinedmass = 0;

    if (nEvents%1000 == 0)
      std::cout << "Event " << nEvents << ", # " << evt.event_number() << ", particle # = " << evt.particles_size() << ", weight # = " << evt.weights().size() << ", passed # = " << passSelection << endl;

    nEvents++;

    weight = evt.weights()[0];
    trials = 1;
    if (evt.weights().size() >= 4) {
      trials = evt.weights()[3];
    }

    TLorentzVector combo(0,0,0,0);
    for (GenEvent::particle_iterator p = evt.particles_begin(); p != evt.particles_end(); ++p) {
      int pid = (int) fabs((*p)->pdg_id());

      // particles from final state
      if ((*p)->status() == 1) {
        TLorentzVector mom(
                      (*p)->momentum().px(),
                      (*p)->momentum().py(),
                      (*p)->momentum().pz(),
                      (*p)->momentum().e());
        //std::cout << "Found particle with PDG ID = " << pid << ", pt = " << mom.Perp() << ", eta = " << mom.Eta() << std::endl;

        if (pid == 11) {
          el_n++;
          el_pt->push_back(mom.Perp());
          el_eta->push_back(mom.Eta());
          el_phi->push_back(mom.Phi());
          el_m->push_back(mom.M());
          combo += mom;
        }
      } // end if final-states
    }
    combinedmass = combo.M();

    // calculate the number of events used:
    passSelection += 1;

    tree->Fill();

    //ascii_in >> evt;
  }
  std::cout << "Number of events that passed selection = " << passSelection << " of a total of " << nEvents << std::endl;

  outFile->Write();
  outFile->Close();
  return 0;
}

