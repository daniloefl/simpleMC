/**
 * \class Unweight
 * \ingroup simpleMC
 * \brief Implements the unweighing procedure, on which
 * an event is dropped deppending on its weight, to generate a distribution
 * of events that follows the probabilidy density function of weights.
 */

#ifndef UNWEIGHT_H
#define UNWEIGHT_H

#include "Process.h"
#include "Selectors.h"
#include "FourVector.h"

class Unweight {
  public:
    /// \brief Constructor
    /// \param p A process used to get the weights (which will be the matrix elements)
    /// \param step Unused now
    Unweight(Process &p, double step = 1e-3);

    /// Destructor
    virtual ~Unweight();

    /// \brief Get maximum weight
    /// \param b1 First beam vector
    /// \param b2 Second beam vector
    /// \param n Number of MC iterations to run over to find maximum
    /// \param select Class that implement selection cuts
    double getMax(FourVector b1, FourVector b2, int n, Selector &select);

    /// \brief Whether this events passes the unweighting procedure
    /// \param mcWeight The matrix element of this event
    /// \return True if a random number is bigger than the matrix element over the maximum weight
    bool accept(double mcWeight);
  private:
    Process &m_p;  ///< Object that returns the matrix element for this scattering
    double m_max;  ///< Maximum matrix element estimate
    double m_step; ///< Unused
};


#endif

