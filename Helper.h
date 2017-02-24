#ifndef HELPER_H
#define HELPER_H

#include <valarray>
#include <complex>

typedef std::complex<double> number;
typedef std::valarray<number> matrix;

class mmatrix {
  public:
    matrix m;
    int i;
    int j;
    mmatrix(int ii = 1, int jj = 1);
    mmatrix operator *(const mmatrix &b) const;
    number toNumber() const;
};

extern double a_s;
extern double a_e;

extern number cI;
extern matrix cg_mu_nu;

double randomDouble();
double alpha_QED(double scale);

matrix gamma(int mu);

matrix mul44_44(const matrix &a, const matrix &b);
matrix mul14_44(const matrix &a, const matrix &b);
number mul14_41(const matrix &a, const matrix &b);
matrix mul44_41(const matrix &a, const matrix &b);
matrix muln_44(number n, const matrix &b);
matrix id_44(number n);

void init();

#endif

