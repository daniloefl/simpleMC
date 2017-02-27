#include "Process.h"

#include <iostream>
#include <vector>

#include "FourVector.h"
#include "ThreeVector.h"

#include "Helper.h"

using namespace std;

int Process::getNdof() {
  return 2;
}

std::vector<double> Process::getRandom() {
  std::vector<double> o;
  for (int l = 0; l < getNdof(); ++l) o.push_back(randomDouble());
  return o;
}


double Process::getSigma(FourVector b1, FourVector b2, int n, double &error, Selector &select) {
  std::cout << "Calculating the cross section ..." << std::endl;
  double sigma = 0;
  double passed = 0;

  std::vector<double> wlist;
  error = 0;
  double mean = 0;

  if (n > 0) {
    for (int k = 0; k < n; ++k) {
      if (k != 0 && k%10000 == 0) {
        error = 0;
        mean = 0;
        for (int z = 0; z < wlist.size(); ++z) {
          mean += wlist[z];
        }
        mean /= (double) wlist.size();
        for (int z = 0; z < wlist.size(); ++z) {
          error += std::pow(wlist[z], 2);
        }
        error /= (double) wlist.size();
        error -= mean;
        if (wlist.size() > 1)
          error /= (double) (wlist.size() - 1);
        error = std::sqrt(error);

        std::cout << "Iteration ["<<k<<"/"<<n<<"] sigma = " << sigma/((double)k) << " +/- " << error << " ("<<100*error/(sigma/((double) k)) <<"%) fb" << std::endl;
      }
      std::vector<FourVector> o;
      double w = 0;
      bool passes = getOut(b1, b2, o, w, select);
      if (passes) {
        passed += 1;
        sigma += w;
        wlist.push_back(w);
      }
    }
    sigma /= (double) n;

    error = 0;
    mean = 0;
    for (int z = 0; z < wlist.size(); ++z) {
      mean += wlist[z];
    }
    mean /= (double) wlist.size();
    for (int z = 0; z < wlist.size(); ++z) {
      error += std::pow(wlist[z], 2);
    }
    error /= (double) wlist.size();
    error -= mean;
    if (wlist.size() > 1)
      error /= (double) (wlist.size() - 1);
    error = std::sqrt(error);
  } else {
    int k = 0;
    error = 999999;
    do {
      if (k != 0 && k%1000 == 0) {
        error = 0;
        mean = 0;
        for (int z = 0; z < wlist.size(); ++z) {
          mean += wlist[z];
        }
        mean /= (double) wlist.size();
        for (int z = 0; z < wlist.size(); ++z) {
          error += std::pow(wlist[z], 2);
        }
        error /= (double) wlist.size();
        error -= mean;
        if (wlist.size() > 1)
          error /= (double) (wlist.size() - 1);
        error = std::sqrt(error);

        std::cout << "Iteration ["<<k<<" (until 0.5\% error)] sigma = " << sigma/((double) k) << " +/- " << error << " ("<<100*error/(sigma/((double)k)) <<"%) fb" << std::endl;
      }
      std::vector<FourVector> o;
      double w = 0;
      bool passes = getOut(b1, b2, o, w, select);
      if (passes) {
        sigma += w;
        wlist.push_back(w);
      }
      k++;
    } while (error/(sigma/((double) k)) > 0.005); // require a 0.5% error
    n = k;
    sigma /= (double) n;

    error = 0;
    mean = 0;
    for (int z = 0; z < wlist.size(); ++z) {
      mean += wlist[z];
    }
    mean /= (double) wlist.size();
    for (int z = 0; z < wlist.size(); ++z) {
      error += std::pow(wlist[z], 2);
    }
    error /= (double) wlist.size();
    error -= mean;
    if (wlist.size() > 1)
      error /= (double) (wlist.size() - 1);
    error = std::sqrt(error);
  }
  return sigma;
}


double Process::getMaximumWeight(FourVector b1, FourVector b2, int n, Selector &select) {
  //std::cout << "Calculating the maximum weight ..." << std::endl;
  double maxi = 0;
  for (int k = 0; k < n; ++k) {
    if (k != 0 && k%1000 == 0) {
      //std::cout << "["<<k<<"/"<<n<<"] max weight = " << maxi << std::endl;
    }
    std::vector<FourVector> o;
    double w = 0;
    bool passes = getOut(b1, b2, o, w, select);
    if (passes && w > maxi) maxi = w;
  }
  return maxi;
}

Process::Process() {
}

Process::~Process() {
}


void Process::getOut2to2(FourVector vb1, FourVector vb2, std::vector<double> &x, std::vector<FourVector> &o) {
  // x values between 0 and 1
  // x vector size is getNdof()
  // use x vector to parametrise output state in 2->2
  // this would be the same for any 2->2 scattering
  // with same particles
  // should only change if there are different masses
  // recalculate it if different masses
  double a = x[0]; // a and b are numbers between 0 and 1
  double b = x[1];

  double phi1 = M_PI*b; // random number between 0 and pi
  double phi2 = 2*M_PI - phi1; // complement (conserves momentum)

  double p = vb1.P(); // same P in initial state and final state (assumes same mass)
  double m = 0.5109e-3; // electron mass (in GeV)
  double theta1 = M_PI*a; // random number between 0 and pi
  double theta2 = M_PI - theta1;
  double eta1 = -std::log(std::tan(theta1*0.5));
  double eta2 = -eta1;

  FourVector vo1;
  vo1.SetPtEtaPhiM(p*std::sin(theta1), eta1, phi1, m);

  FourVector vo2;
  vo2.SetPtEtaPhiM(p*std::sin(theta2), eta2, phi2, m);

  o.push_back(vo1);
  o.push_back(vo2);

  // this is the end of the final state calculation
  // this can be always the same for 2->2 processes so far (with adaptations in case of different masses)
  // the amplitude, next would be different though
  
}

double Process::lambda(double a, double b, double c) {
  return std::pow(a - b - c, 2) - 4*b*c;
}

double Process::factorial(double n) {
  if (n <= 1) return 1;
  return factorial(n-1);
}

double Process::getRAMBOPhaseSpace(FourVector vb1, FourVector vb2, std::vector<FourVector> &o, std::vector<double> &masses, std::vector<int> &pid) {
  double Ecm = vb1.E() + vb2.E();
  int n = masses.size();
  if (masses.size() != pid.size()) return -1;
  
  double ps = 0;
  // generate n output particles
  std::vector<FourVector> qv;
  FourVector Q(0,0,0,0);
  double M = 0;
  FourVector b(0,0,0,0);
  double gamma = 0;
  double x = 0;
  double a = 0;
  for (int k = 0; k < n; ++k) {
    double rho_1 = randomDouble();
    double rho_2 = randomDouble();
    double rho_3 = randomDouble();
    double rho_4 = randomDouble();
    double c = 2*rho_1 - 1;
    double phi = 2*M_PI*rho_2;
    double q0 = -std::log(rho_3*rho_4);
    double qx = q0*std::sqrt(1-c*c)*std::cos(phi);
    double qy = q0*std::sqrt(1-c*c)*std::sin(phi);
    double qz = q0*c;
    qv.push_back(FourVector(qx,qy,qz,q0));
    Q += qv[qv.size()-1];
  }
  M = Q.M();
  b = -Q*(1.0/M);
  gamma = Q.E()/M;
  x = Ecm/M;
  a = 1.0/(1.0+gamma);
  std::vector<FourVector> plist;
  for (int k = 0; k < n; ++k) {
    double p0 = x*(gamma*qv[k].E() + b.Vect().Dot(qv[k].Vect()));
    double px = x*(qv[k].Px() + b.Px()*qv[k].E() + a*(b.Vect().Dot(qv[k].Vect()))*b.Px());
    double py = x*(qv[k].Py() + b.Py()*qv[k].E() + a*(b.Vect().Dot(qv[k].Vect()))*b.Py());
    double pz = x*(qv[k].Pz() + b.Pz()*qv[k].E() + a*(b.Vect().Dot(qv[k].Vect()))*b.Pz());
    plist.push_back(FourVector(px,py,pz, p0));
  }
  // this is the wright from the RAMBO paper
  // it gives a phase space element of the form
  // dPS = \int \delta^4(P - sum p_i) \prod d^{4}p_i \delta(p_i^2) \theta(p_i^0)
  ps = std::pow(M_PI/2.0, n - 1)*std::pow(Ecm,2*n-4)/((double) (n-1) * std::pow(factorial(n-2),2));
  // the phase space assumed for the cross section calculation here is
  // (2pi)^4 \delta^4(P - sum p_i) \prod d^3p_i/((2pi)^3 * 2E_i)
  // there is a ratio of a factor of (2pi)^(4-3n)
  // apply it now
  ps *= std::pow(2*M_PI,4-3*n);

  // add masses here
  double xi = 0.9; // find solution for this
  double func = 9999;
  double dfunc = 1;
  int iter = 0;
  do {
    func = - Ecm;
    for (int k = 0; k < n; ++k) {
      func += std::sqrt(masses[k]*masses[k] + xi*xi*(plist[k].E()*plist[k].E() - masses[k]*masses[k]));
    }
    dfunc = 0;
    for (int k = 0; k < n; ++k) {
      dfunc += xi*(plist[k].E()*plist[k].E() - masses[k]*masses[k])/std::sqrt(masses[k]*masses[k] + xi*xi*(plist[k].E()*plist[k].E() - masses[k]*masses[k]));
    }
    xi -= func/dfunc;
    iter++;
  } while (std::fabs(func) > 1e-10 && iter < 20);

  double mps = ps*std::pow(xi,3*(n-1));
  double sumpr = 0;
  double sumkr = 0;
  for (int k = 0; k < n; ++k) {
    double px = plist[k].Px();
    double py = plist[k].Py();
    double pz = plist[k].Pz();
    double p0 = plist[k].E();
    double kx = xi*px;
    double ky = xi*py;
    double kz = xi*pz;
    double k0 = std::sqrt(std::pow(masses[k],2) + std::pow(xi,2)*(std::pow(p0,2) - std::pow(masses[k],2)));;
    //o.push_back(GenParticle(FourVector(kx,ky,kz, k0),pid[k],1));
    o.push_back(FourVector(px, py, pz, p0)); // output particle
    mps *= (p0/k0);
    sumpr += (std::pow(px,2) + std::pow(py,2) + std::pow(pz,2))/p0;
    sumkr += (std::pow(kx,2) + std::pow(ky,2) + std::pow(kz,2))/k0;
  }
  mps *= sumpr/sumkr;

  return mps;
}

double Process::getFlux(FourVector vb1, FourVector vb2, std::vector<FourVector> &o) {
  FourVector vo1 = o[0];

  // it only works for massless particles in 2->2!
  //double ps = 1.0/(8*M_PI); // only for 2 particles in FS

  // include the flux factor
  double beam1mass = vb1.M();
  double beam2mass = vb2.M();
  double flux = 1.0/(4*std::sqrt(std::pow(vb1.Dot(vb2),2) - std::pow(beam1mass*beam2mass, 2)));
  // random numbers are in space of theta and phi, in [0,pi]
  // now it has units of hbar^2*c^2/GeV^2
  // convert to fb
  double hbarc = (1.23984193/(2*M_PI)); // hbar*c = 0.197 GeV * fm
  // 1 barn = 10^-28 m^2 = 100 fm^2
  // 1 fb = 10^-15 barn = 10^-13 fm^2
  // 1 fm^2 = 10^13 fb;
  // convert everything from GeV^-2 to fm^2:
  flux *= (hbarc*hbarc);
  // convert it to fb
  flux *= 1e13;
  return flux;
}

bool Process::getOut(FourVector b1, FourVector b2, std::vector<FourVector> &o, double &w, Selector &select) {
  // old code only 2->2
  //std::vector<double> x = getRandom();
  //getOut2to2(b1, b2, x, o); // gets output state
  //double ps = 1.0/(8*M_PI);

  // based on the RAMBO paper
  // PID and masses of the output particles
  // TODO (read it from user)
  std::vector<double> masses;
  std::vector<int> pid;
  masses.push_back(0.5109e-3); pid.push_back(11);
  masses.push_back(0.5109e-3); pid.push_back(-11);
  double ps = getRAMBOPhaseSpace(b1, b2, o, masses, pid);

  // select final state
  if (!select(o)) return false;
  // get |M|^2
  w = getAmp(b1, b2, o);
  // multiply by flux factor
  w *= getFlux(b1, b2, o);
  w *= ps;      // multiply by phase space weight
  return true;
}

