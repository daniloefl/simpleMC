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
    double getAmp(FourVector b1, FourVector b2, std::vector<FourVector> &o);

};

#endif

