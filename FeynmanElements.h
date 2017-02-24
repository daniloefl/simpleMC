#include "Helper.h"

#include <valarray>
#include <complex>
#include <vector>

#include "TLorentzVector.h"

using namespace std;

inline mmatrix fdm_QEDPropagatorZ(TLorentzVector &p, int mu, int nu) {
  static double M_z = 91.1876;
  static double L_z = 2.4952;
  double pmu = 0;
  double pnu = 0;
       if (mu == 0) pmu = p.E();
  else if (mu == 1) pmu = p.Px();
  else if (mu == 2) pmu = p.Py();
  else if (mu == 3) pmu = p.Pz();
       if (nu == 0) pnu = p.E();
  else if (nu == 1) pnu = p.Px();
  else if (nu == 2) pnu = p.Py();
  else if (nu == 3) pnu = p.Pz();
  double g = 0;
  if (mu == nu) g = 1;
  if (mu > 0) g = -g;
  number r(0, -g + pmu*pnu/(M_z*M_z));
  number d(p.M2() - M_z*M_z, M_z*L_z);
  r /= d;
  mmatrix rm(1,1);
  rm.m[0] = r;
  return rm;
}

inline mmatrix fdm_QEDVertexZ(double scale, double ch, int s, int mu) {
  static double theta_W = asin(sqrt(0.2397));
  static double v_f2 = fabs(ch)*pow(sin(theta_W),2);
  static double sincos = (sin(theta_W)*cos(theta_W));
  
  double T32 = -0.25;
  if (s == 1) T32 = -0.5;
  double a_f = T32;
  double v_f = T32 - v_f2;

  number r(0, sqrt(4*M_PI*alpha_QED(scale))/sincos);
  matrix gm = gamma(mu)*r;
  matrix g2 = id_44(v_f) - gamma(5)*number(a_f,0);
  mmatrix rm(4,4);
  rm.m = mul44_44(gm,g2);
  return rm;
}

inline mmatrix fdm_QEDVertex(double scale, double ch, int mu) {
  number r(0, -std::sqrt(4*M_PI*alpha_QED(scale))*fabs(ch));
  mmatrix rm(4,4);
  rm.m = gamma(mu)*r;
  return rm;
}

inline mmatrix fdm_QEDPropagatorA(double p2, int mu, int nu) {
  if (mu != nu) return 0;
  if (mu == 0) return number(0, -1.0/p2);
  mmatrix rm(1,1);
  rm.m[0] = number(0, +1.0/p2);
  return rm;
}

inline mmatrix fdm_Fermion(bool f, bool anti, int s, TLorentzVector &p, double mass = 0.5109e-3) {
  matrix r(number(0,0), 4);
  double x = std::sqrt(p.E() + mass);
  double invx = 1.0/x;
  if (!anti) { // u(p)
    if (s == 0) { // spin up
      r[0] = x;
      //r[1] = 0;
      r[2] = p.Pz()*invx;
      r[3] = (p.Px()+ cI*p.Py())*invx;
    } else { // spin down
      //r[0] = 0;
      r[1] = x;
      r[2] = (p.Px()- cI*p.Py())*invx;
      r[3] = -p.Pz()*invx;
    }
  } else {     // v(p)
    if (s == 0) { // spin up
      r[0] = (p.Px()- cI*p.Py())*invx;
      r[1] = -p.Pz()*invx;
      //r[2] = 0;
      r[3] = x;
    } else { // spin down
      r[0] = p.Pz()*invx;
      r[1] = (p.Px()+ cI*p.Py())*invx;
      r[2] = x;
      //r[3] = 0;
    }
  }
  mmatrix rm(1,4);
  if ((f && !anti) || (!f && anti)) { // take the conjugate
    // take hermitian and then multiply by Gamma(0)
    for (int i = 0; i < 4; ++i) r[i] = conj(r[i]);
    r[2] = -r[2];
    r[3] = -r[3];
    rm.i = 4;
    rm.j = 1;
  }
  rm.m = r;
  return rm;
}

