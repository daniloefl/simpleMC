#include "BhabhaProcess.h"
#include "Process.h"
#include "Helper.h"

#include <vector>
#include "HepMC/GenParticle.h"

#include "Helper.h"

#include "TLorentzVector.h"

using namespace std;
using namespace HepMC;


BhabhaProcess::BhabhaProcess()
  : Process() {
}

BhabhaProcess::~BhabhaProcess() {
}

int BhabhaProcess::getNdof() {
  return 2; // e+ e- > e+ e- has N_out = 2 -> 4*N - N - 4 = 2
}

double BhabhaProcess::getAmp(GenParticle *b1, GenParticle *b2, std::vector<GenParticle> &o) {

  TLorentzVector vb1(b1->momentum().px(), b1->momentum().py(), b1->momentum().pz(), b1->momentum().e());
  TLorentzVector vb2(b2->momentum().px(), b2->momentum().py(), b2->momentum().pz(), b2->momentum().e());

  TLorentzVector vo1(o[0].momentum().px(), o[0].momentum().py(), o[0].momentum().pz(), o[0].momentum().e());
  TLorentzVector vo2(o[1].momentum().px(), o[1].momentum().py(), o[1].momentum().pz(), o[1].momentum().e());

  double s = ((vb1+vb2).M2());
  double t = ((vb1+vo1).M2());
  double u = ((vb1+vo2).M2());

  // process specific
  // alpha_QED = fine structure constant = e^2/(4*pi) in natural units
  double ec = std::sqrt(4*M_PI*alpha_QED(s)); // electron charge

  double amp = 2*std::pow(ec, 4);
  amp *= (u*u +s*s)/(t*t) + 2*u*u/(s*t) + (u*u + t*t)/(s*s);
  return amp;
}

