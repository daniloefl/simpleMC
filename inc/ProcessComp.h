#ifndef PROCESSCOMP_H
#define PROCESSCOMP_H

#include "Process.h"
#include <vector>

// test process
// e+ e- -> gamma -> e+ e-
class ProcessComp : public Process {
  public:
    ProcessComp();
    virtual ~ProcessComp();

    int getNdof();

    // calculates |M|^2 for e+ e- -> e+ e-
    double getAmp(FourVector b1, FourVector b2, std::vector<FourVector> &o);

};

#endif

