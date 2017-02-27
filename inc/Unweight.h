#ifndef UNWEIGHT_H
#define UNWEIGHT_H

#include "Process.h"
#include "Selectors.h"
#include "FourVector.h"

class Unweight {
  public:
    Unweight(Process &p, double step = 1e-3);
    virtual ~Unweight();

    double getMax(FourVector b1, FourVector b2, int n, Selector &select);
    bool accept(double mcWeight);
  private:
    Process &m_p;
    double m_max;
    double m_step;
};


#endif

