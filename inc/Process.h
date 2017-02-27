#ifndef PROCESS_H
#define PROCESS_H

#include <vector>
#include "Selectors.h"

#include "FourVector.h"

// Bhabha scattering
// could be a general class for any process
class Process {
  public:
    Process();
    virtual ~Process();

    // number of output degrees of freedom = 4*N - N -4
    // for N final state particles
    // 4*N = each particle has 4 components in the four-vector
    // -N  = they are all on-shell (their mass is known)
    // -4  = the system conserves energy-momentum (4 conservation equations)
    // this should be overwritten depending if the processes is 2->2, 2->3, etc
    virtual int getNdof();

    // returns a vector of random numbers between 0 and 1
    // the size of the vector is getNdof()
    // does not need to be overwritten
    virtual std::vector<double> getRandom();

    double factorial(double n); // calculates factorial of n (integer)

    // generates output in case of a e+ e- -> e+ e-
    // with uniform random theta_1 and phi_1 between 0 and pi
    // this needs to be generalised for any other process (not only ee -> ee)
    virtual void getOut2to2(FourVector b1, FourVector b2, std::vector<double> &x, std::vector<FourVector> &o);

    // calculates |M|^2 for e+ e- -> e+ e-
    // this should be overwritten for other processes
    virtual double getAmp(FourVector b1, FourVector b2, std::vector<FourVector> &o) = 0;

    protected:
    // auxiliary function for PS calculation
    double lambda(double a, double b, double c);

    public:
    // get flux factor
    virtual double getFlux(FourVector b1, FourVector b2, std::vector<FourVector> &o);

    // use RAMBO to generate the phase space
    virtual double getRAMBOPhaseSpace(FourVector b1, FourVector b2, std::vector<FourVector> &o, std::vector<double> &masses, std::vector<int> &pid);

    // this should generate uniform output state (ie: with getRAMBOPhaseSpace)
    // and calculate the amplitude as a weight with getAmp
    // and then multiply it by the flux factor with getFlux
    virtual bool getOut(FourVector b1, FourVector b2, std::vector<FourVector> &o, double &w, Selector &select);

    // integrate cross section
    virtual double getSigma(FourVector b1, FourVector b2, int n, double &error, Selector &select);

    // get maximum weight
    virtual double getMaximumWeight(FourVector b1, FourVector b2, int n, Selector &select);
};


#endif
