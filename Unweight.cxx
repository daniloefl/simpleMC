#include "Unweight.h"

#include "HepMC/GenParticle.h"
#include "Selectors.h"
#include "Process.h"
#include "Helper.h"

using namespace HepMC;

Unweight::Unweight(Process &p, double step)
  : m_p(p), m_max(-1), m_step(step) {
}

Unweight::~Unweight() {
}

double Unweight::getMax(GenParticle *b1, GenParticle *b2, int n, Selector &select) {
  if (m_max > 0) return m_max;
  // could just sweep functions to get maximum
  // try MC method
  m_max = m_p.getMaximumWeight(b1, b2, n, select);
  return m_max;
}

bool Unweight::accept(double mcWeight) {
  if (randomDouble() > mcWeight/m_max) {
    return false;
  }
  return true;
}


