#ifndef SELECTORS_H
#define SELECTORS_H

#include <vector>
#include "FourVector.h"

class Selector {
  public:
    Selector();
    virtual ~Selector();
    virtual bool operator()(std::vector<FourVector> &o) = 0;
};


class MySelector : public Selector {
  public:
    MySelector();
    virtual ~MySelector();
    bool operator()(std::vector<FourVector> &o);
};

#endif

