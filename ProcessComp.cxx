#include "ProcessComp.h"
#include "Process.h"
#include "Helper.h"

#include <valarray>
#include <complex>
#include <vector>
#include "HepMC/GenParticle.h"

#include "Helper.h"

#include "TLorentzVector.h"

using namespace std;
using namespace HepMC;

inline number fd_QEDPropagatorZ(TLorentzVector &p, int mu, int nu) {
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
  return r;
}

inline matrix fd_QEDVertexZ(double scale, double ch, int s, int mu) {
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
  return mul44_44(gm,g2);
}

inline matrix fd_QEDVertex(double scale, double ch, int mu) {
  number r(0, -std::sqrt(4*M_PI*alpha_QED(scale))*fabs(ch));
  matrix gm = gamma(mu)*r;
  return gm;
}

inline number fd_QEDPropagatorA(double p2, int mu, int nu) {
  if (mu != nu) return 0;
  if (mu == 0) return number(0, -1.0/p2);
  return number(0, +1.0/p2);
}

inline matrix fd_Fermion(bool f, bool anti, int s, TLorentzVector &p, double mass = 0.5109e-3) {
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
  if ((f && !anti) || (!f && anti)) { // take the conjugate
    // take hermitian and then multiply by Gamma(0)
    for (int i = 0; i < 4; ++i) r[i] = conj(r[i]);
    r[2] = -r[2];
    r[3] = -r[3];
  }
  return r;
}

ProcessComp::ProcessComp()
  : Process() {
init();
}

ProcessComp::~ProcessComp() {
}

int ProcessComp::getNdof() {
  return 2; // a b > c d has N_out = 2 -> 4*N - N - 4 = 2
}

double ProcessComp::getAmp(GenParticle *b1, GenParticle *b2, std::vector<GenParticle> &o) {

  TLorentzVector vb1(b1->momentum().px(), b1->momentum().py(), b1->momentum().pz(), b1->momentum().e());
  TLorentzVector vb2(b2->momentum().px(), b2->momentum().py(), b2->momentum().pz(), b2->momentum().e());

  TLorentzVector vo1(o[0].momentum().px(), o[0].momentum().py(), o[0].momentum().pz(), o[0].momentum().e());
  TLorentzVector vo2(o[1].momentum().px(), o[1].momentum().py(), o[1].momentum().pz(), o[1].momentum().e());

  std::vector<TLorentzVector> in;
  std::vector<TLorentzVector> out;
  in.push_back(vb1);
  in.push_back(vb2);
  out.push_back(vo1);
  out.push_back(vo2);

  double scale = (in[0]+in[1]).M2(); // not used yet (alpha_QED not running)
  TLorentzVector p = in[0]+in[1];
  TLorentzVector p2 = in[0]-out[0];
  double charge = -1;

  int s1 = 0; // initial state spins
  int s2 = 0;
  int r1 = 0; // final state spins
  int r2 = 0;
  number M2 = 0;
  matrix v[4];
  matrix vz[4];
  matrix vz2[4];
  for (int k = 0; k < 4; ++k) {
    v[k] = fd_QEDVertex(scale, charge, k);
  }
  for (s1 = 0; s1 < 2; ++s1) { // average over initial state spins (factor of 1/4 below)
    matrix fi0 = fd_Fermion(false, true, s1, in[0]);
    for (s2 = 0; s2 < 2; ++s2) { // average over initial state spins (factor of 1/4 below)
      matrix fi1 = fd_Fermion(false, false, s2, in[1]);
      for (r1 = 0; r1 < 2; ++r1) { // sum over final state spins
        matrix fo1 = fd_Fermion(true, true, r1, out[0]);
        for (r2 = 0; r2 < 2; ++r2) { // sum over final state spins
          matrix fo0 = fd_Fermion(true, false, r2, out[1]);
          // include photon interactions
          number M = 0;
          for (int mu = 0; mu < 4; mu++) {
            for (int nu = 0; nu < 4; nu++) {
              M += mul14_41(mul14_44(fi0, v[mu]), fi1)*fd_QEDPropagatorA(p.M2(), mu, nu)*(mul14_41(mul14_44(fo0, v[nu]), fo1));
              M += mul14_41(mul14_44(fi0, v[mu]), fo1)*fd_QEDPropagatorA(p2.M2(), mu, nu)*(mul14_41(mul14_44(fo0, v[nu]), fi1));
            }
          }

          // include Z interactions
          if (s1 == s2 && r1 == r2) {
            for (int k = 0; k < 4; ++k) {
              vz[k] = fd_QEDVertexZ(scale, charge, s1, k);
              vz2[k] = fd_QEDVertexZ(scale, charge, r2, k);
            }
            for (int mu = 0; mu < 4; mu++) {
              for (int nu = 0; nu < 4; nu++) {
                M += mul14_41(mul14_44(fi0, vz[mu]), fi1)*fd_QEDPropagatorZ(p, mu, nu)*(mul14_41(mul14_44(fo0, vz2[nu]), fo1));//(i1->term()*vz->term()*i2->term())*(propz->term())*(o2->term()*vz2->term()*o1->term());
              }
            }
          }
          if (s1 == r1 && s2 == r2) {
            for (int k = 0; k < 4; ++k) {
              vz[k] = fd_QEDVertexZ(scale, charge, s1, k);
              vz2[k] = fd_QEDVertexZ(scale, charge, r2, k);
            }
            for (int mu = 0; mu < 4; mu++) {
              for (int nu = 0; nu < 4; nu++) {
                M += mul14_41(mul14_44(fi0, vz[mu]), fo1)*fd_QEDPropagatorZ(p2, mu, nu)*(mul14_41(mul14_44(fo0, vz2[nu]), fi1));//(i1->term()*vz->term()*o1->term())*(propz2->term())*(o2->term()*vz2->term()*i2->term());
              }
            }
          }

          M2 += norm(M);
        }
      }
    }
  }
  M2 *= 0.25; // average over spins in initial state

  double amp = M2.real();

  return amp;
}

