/**
 * \class Process
 * \ingroup simpleMC
 * \brief Base class to model a scattering, to be overridden with a class that provides an implementation of
 * getAmp
 */

#ifndef PROCESS_H
#define PROCESS_H

#include <vector>
#include "Selectors.h"

#include "FourVector.h"

class Process {
  public:

    /// Constructor
    Process();

    /// Destructor
    virtual ~Process();

    /// \brief Returns the number of degrees of freedom for a scattering.
    /// Number of output degrees of freedom = 4*N - N -4
    /// For N final state particles
    /// 4*N = each particle has 4 components in the four-vector
    /// -N  = they are all on-shell (their mass is known)
    /// -4  = the system conserves energy-momentum (4 conservation equations)
    /// This should be overwritten depending if the processes is 2->2, 2->3, etc
    /// \return ndof
    virtual int getNdof();

    /// \brief Returns a vector of random numbers between 0 and 1
    /// The size of the vector is getNdof()
    /// Does not need to be overwritten
    /// \return Random vector of numbers with length given by ndof.
    virtual std::vector<double> getRandom();

    /// \brief Get factorial of a number
    /// \param n Number to take factorial from
    /// \return Factorial of n
    double factorial(double n);

    /// \brief Generates output vectors in case of a e+ e- -> e+ e-
    ///        with uniform random theta_1 and phi_1 between 0 and pi.
    ///        This needs to be generalised for any other process (not only ee -> ee)
    /// \param b1 First beam vector
    /// \param b2 Second beam vector
    /// \param x Vector with two elements that are random numbers between 0 and 1: this uniquely specifies the final state
    /// \param o Vector of output 4-vectors to be returned by reference.
    virtual void getOut2to2(FourVector b1, FourVector b2, std::vector<double> &x, std::vector<FourVector> &o);

    /// \brief Calculates matrix element.
    ///        This should be overwritten for other processes
    /// \param b1 First beam vector.
    /// \param b2 Second beam vector.
    /// \param o Final state four-vectors.
    virtual double getAmp(FourVector b1, FourVector b2, std::vector<FourVector> &o) = 0;

    protected:
    /// \brief Auxiliary function for PS calculation
    double lambda(double a, double b, double c);

    public:
    /// \brief Get flux factor for cross section calculation.
    /// \param b1 First beam vector
    /// \param b2 Second beam vector
    /// \param o Output four-vectors.
    /// \return Flux factor.
    virtual double getFlux(FourVector b1, FourVector b2, std::vector<FourVector> &o);

    /// \brief Get phase space element using RAMBO
    /// \param b1 First beam vector.
    /// \param b2 Second beam vector.
    /// \param o Final state four-vectors.
    /// \param masses of the final state particles.
    /// \param pid Particle identifiers of rht efinal state particles (currently ignored)
    /// \return Phase space element factor.
    virtual double getRAMBOPhaseSpace(FourVector b1, FourVector b2, std::vector<FourVector> &o, std::vector<double> &masses, std::vector<int> &pid);

    /// \brief Generates a uniform output state using RAMBO,
    ///        calculate the amplitude from getAmp(), multiply it by the flux factor from getFlux() and
    ///        apply selection from select
    /// \param b1 First beam vector.
    /// \param b2 Second beam vector.
    /// \param o Final state four-vectors.
    /// \param w Weight to be returned by reference.
    /// \param select Selector to apply selection on the event
    /// \return Whether the event passes the selection.
    virtual bool getOut(FourVector b1, FourVector b2, std::vector<FourVector> &o, double &w, Selector &select);

    /// \brief Integrates the cross section by getting many output weights and summing them.
    /// \param b1 First beam vector.
    /// \param b2 Second beam vector.
    /// \param n Number of MC iterations.
    /// \param error Error of cross section calculation -- to be returned by reference.
    /// \param select Selector to be applied before cross section estimate.
    /// \return Cross section estimate.
    virtual double getSigma(FourVector b1, FourVector b2, int n, double &error, Selector &select);

    /// \brief Get maximum weight to apply unweighting
    /// \param b1 First beam vector.
    /// \param b2 Second beam vector.
    /// \param n Number of MC iterations.
    /// \param select Selector to be applied before cross section estimate.
    /// \return Maximum weight found in n iterations.
    virtual double getMaximumWeight(FourVector b1, FourVector b2, int n, Selector &select);
};

#endif

