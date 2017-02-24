#ifndef BHABHAPROCESS_H
#define BHABHAPROCESS_H

#include "Process.h"
#include <vector>

// test process
// e+ e- -> gamma -> e+ e-
class BhabhaProcess : public Process {
  public:
    BhabhaProcess();
    virtual ~BhabhaProcess();

    int getNdof();

    // calculates |M|^2 for e+ e- -> e+ e-
    double getAmp(TLorentzVector b1, TLorentzVector b2, std::vector<TLorentzVector> &o);

};

#endif

