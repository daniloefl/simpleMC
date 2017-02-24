#include "Helper.h"

#include "TRandom.h"
#include "TRandom3.h"

#include <cmath>
#include <valarray>

using namespace std;

double a_s = 0.118;
//double a_e = 1.0/137.0;

number cI(0,1);

double randomDouble() {
  static TRandom3 r;
  return r.Uniform(0, 1); // get uniformly distributed number
}

double alpha_QED(double scale) {
  // TODO add running of coupling
  double a_e = 1.0/137.0;
  double beta = a_e/(3*M_PI);
  double mu_R = 91*91; // MZ
  return a_e/(1-beta*std::log(scale/mu_R));
}

matrix g0(number(0,0), 4*4);
matrix g1(number(0,0), 4*4);
matrix g2(number(0,0), 4*4);
matrix g3(number(0,0), 4*4);
matrix g5(number(0,0), 4*4);

void init() {
    g0[slice(0, 2, 4+1)] = 1;
    g0[slice(2*4+2, 2, 4+1)] = -1;
    g1[slice(3, 2, 3)] = 1;
    g1[slice(3*3, 2, 3)] = -1;
    g2[3] = -cI;
    g2[6] = cI;
    g2[9] = cI;
    g2[12] = -cI;
    g3[2] = 1;
    g3[7] = -1;
    g3[8] = -1;
    g3[13] = 1;
    g5[2] = 1;
    g5[7] = 1;
    g5[8] = 1;
    g5[13] = 1;
}

mmatrix::mmatrix(int ii, int jj)
  : m(number(0,0), ii*jj) {
  i = ii;
  j = jj;
}

mmatrix mmatrix::operator *(const mmatrix &b) const {
  matrix r(number(0,0), 1);
  if (i == 1 && j == 1 && b.i == 1 && b.j == 1)
    r[0] = this->m[0]*b.m[0];
  else if (i == 1 && j == 4 && b.i == 4 && b.j == 4)
    r = mul14_44(this->m, b.m);
  else if (i == 4 && j == 4 && b.i == 4 && b.j == 4)
    r = mul44_44(this->m, b.m);
  else if (i == 1 && j == 4 && b.i == 4 && b.j == 1)
    r = mul14_41(this->m, b.m);
  else if (i == 4 && j == 4 && b.i == 4 && b.j == 1)
    r = mul44_41(this->m, b.m);
  r.i = i;
  r.j = b.j;
  return r;
}

number mmatrix::toNumber() const {
  return m[0];
}

matrix gamma(int mu) {
  if (mu == 0) {
    return g0;
  } else if (mu == 1) {
    return g1;
  } else if (mu == 2) {
    return g2;
  } else if (mu == 3) {
    return g3;
  } else if (mu == 5) {
    return g5;
  }
  return g5;
}

matrix mul44_44(const matrix &a, const matrix &b) {
  matrix r(number(0,0),4*4);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; ++j) {
      for (int k = 0; k < 4; ++k) {
        r[i*4+j] += a[i*4+k]*b[k*4+j];
      }
    }
  }
  return r;
}

number mul14_41(const matrix &a, const matrix &b) {
  number r = 0;
  for (int i = 0; i < 4; i++) {
    r += a[i]*b[i];
  }
  return r;
}

matrix mul14_44(const matrix &a, const matrix &b) {
  matrix r(number(0,0),4);
  for (int i = 0; i < 4; i++) {
    for (int k = 0; k < 4; ++k) {
      r[i] += a[k]*b[k*4+i];
    }
  }
  return r;
}

matrix mul44_41(const matrix &a, const matrix &b) {
  matrix r(number(0,0),4);
  for (int i = 0; i < 4; i++) {
    for (int k = 0; k < 4; ++k) {
      r[i] += a[i*4+k]*b[k];
    }
  }
  return r;
}

matrix id_44(number n) {
  matrix r(number(0,0),4*4);
  r[slice(0,4,4+1)] = n;
  return r;
}

