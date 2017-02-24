#include "BhabhaProcess.h"
#include "Process.h"
#include "Helper.h"

#include <vector>

#include "Helper.h"

#include "TLorentzVector.h"

using namespace std;


BhabhaProcess::BhabhaProcess()
  : Process() {
}

BhabhaProcess::~BhabhaProcess() {
}

int BhabhaProcess::getNdof() {
  return 2; // e+ e- > e+ e- has N_out = 2 -> 4*N - N - 4 = 2
}

double BhabhaProcess::getAmp(TLorentzVector vb1, TLorentzVector vb2, std::vector<TLorentzVector> &o) {

  TLorentzVector vo1 = o[0];
  TLorentzVector vo2 = o[1];

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

