/**
 * \class ProcessComp
 * \ingroup simpleMC
 * \brief Calculates the matrix element of the e+ e- -> Z/\gamma -> e+ e- process
 */

#ifndef PROCESSCOMP_H
#define PROCESSCOMP_H

#include "Process.h"
#include <vector>

class ProcessComp : public Process {
  public:
    /// Constructor
    ProcessComp();

    /// Destructor
    virtual ~ProcessComp();

    /// Get number of degrees of freedom for this matrix element function
    int getNdof();

    /// \brief Calculates |M|^2 for e+ e- -> e+ e-
    /// \param b1 First beam vector
    /// \param b2 Second beam vector
    /// \param o List of final state 4-vectors
    /// \return Matrix element
    double getAmp(FourVector b1, FourVector b2, std::vector<FourVector> &o);

};

#endif

