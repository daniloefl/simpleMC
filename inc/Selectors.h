/**
 * \brief Implements an abstract selector class and a test selector with loose cuts.
 * \ingroup simpleMC
 * 
 * The selector classes have a cast operator that return true or false, depending whether
 * the event is accepted or not, under a specific final state vector configuration.
 */

#ifndef SELECTORS_H
#define SELECTORS_H

#include <vector>
#include "FourVector.h"

class Selector {
  public:
    /// Abstract class Selector's constructor
    Selector();
    /// Destructor
    virtual ~Selector();

    /// \brief Cast operator to be overridden.
    /// \param o Vector of final-state 4-vectors
    /// \return Whether the event is selected or not
    virtual bool operator()(std::vector<FourVector> &o) = 0;
};


class MySelector : public Selector {
  public:
    /// Constructor of sample selector
    MySelector();
    /// Destructor
    virtual ~MySelector();

    /// \brief Implementation of simple selection with loose cuts.
    /// \param o Vector of final-state 4-vectors
    /// \return Whether the event is selected or not
    bool operator()(std::vector<FourVector> &o);
};

#endif

