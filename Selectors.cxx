#include "Selectors.h"
#include "TLorentzVector.h"

using namespace std;

Selector::Selector() {
}

Selector::~Selector() {
}

MySelector::MySelector() {
}

MySelector::~MySelector() {
}

bool MySelector::operator()(std::vector<TLorentzVector> &o) {
  TLorentzVector vo1 = o[0];
  TLorentzVector vo2 = o[1];

  // each final electron has > 5 GeV
  if (vo1.Perp() < 5 || vo2.Perp() < 5) return false;
  if (std::fabs(vo1.Rapidity()) > 5) return false;
  if (std::fabs(vo2.Rapidity()) > 5) return false;

  //if (vo1.Perp() >= 10 || vo2.Perp() >= 10) return false;
 

  // 5 < theta < 175 cuts
  //double ThetaCut1 = M_PI/180.0*5;
  //double ThetaCut2 = M_PI/180.0*175;
  //if (std::fabs(vo1.Theta()) < ThetaCut1 || std::fabs(vo1.Theta()) > ThetaCut2) return false;
  //if (std::fabs(vo2.Theta()) < ThetaCut1 || std::fabs(vo2.Theta()) > ThetaCut2) return false;

  return true;
}

