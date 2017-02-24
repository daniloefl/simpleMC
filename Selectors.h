#ifndef SELECTORS_H
#define SELECTORS_H

#include <vector>
#include "TLorentzVector.h"

class Selector {
  public:
    Selector();
    virtual ~Selector();
    virtual bool operator()(std::vector<TLorentzVector> &o) = 0;
};


class MySelector : public Selector {
  public:
    MySelector();
    virtual ~MySelector();
    bool operator()(std::vector<TLorentzVector> &o);
};

#endif

