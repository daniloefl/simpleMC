#ifndef PROCESSCOMP_H
#define PROCESSCOMP_H

#include "Process.h"
#include <vector>
#include "HepMC/GenParticle.h"

// test process
// e+ e- -> gamma -> e+ e-
class ProcessComp : public Process {
  public:
    ProcessComp();
    virtual ~ProcessComp();

    int getNdof();

    // calculates |M|^2 for e+ e- -> e+ e-
    double getAmp(HepMC::GenParticle *b1, HepMC::GenParticle *b2, std::vector<HepMC::GenParticle> &o);

};

#endif

