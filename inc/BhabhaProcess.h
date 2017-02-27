/**
 * \class BhabhaProcess
 *
 * \ingroup simpleMC
 *
 * \brief Calculates the matrix element for e+ e- -> \gamma -> e+ e-.
 *
 */

#ifndef BHABHAPROCESS_H
#define BHABHAPROCESS_H

#include "Process.h"
#include <vector>

class BhabhaProcess : public Process {
  public:

    /// Constructor
    BhabhaProcess();
    /// Destructor
    virtual ~BhabhaProcess();

    /// Get number of degrees of freedom in this ME calculation
    int getNdof();

    /// \brief Calculates |M|^2 for e+ e- -> e+ e-
    /// \param b1 First beam vector
    /// \param b2 Second beam vector
    /// \param o List of output vectors
    /// \return Matrix element
    double getAmp(FourVector b1, FourVector b2, std::vector<FourVector> &o);

};

#endif

