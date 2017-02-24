#ifndef UNWEIGHT_H
#define UNWEIGHT_H

#include "Process.h"
#include "HepMC/GenParticle.h"
#include "Selectors.h"

class Unweight {
  public:
    Unweight(Process &p, double step = 1e-3);
    virtual ~Unweight();

    double getMax(HepMC::GenParticle *b1, HepMC::GenParticle *b2, int n, Selector &select);
    bool accept(double mcWeight);
  private:
    Process &m_p;
    double m_max;
    double m_step;
};


#endif
