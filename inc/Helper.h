/**
 * \brief Set of helper functions.
 * \ingroup simpleMC
 */

#ifndef HELPER_H
#define HELPER_H

#include <valarray>
#include <complex>

/// Defines a complex number
typedef std::complex<double> number;

/// Defines a 4x4 matrix of complex numbers
typedef std::valarray<number> matrix;

/// alpha_strong
extern double a_s;

/// alpha_electroweak
extern double a_e;

/// sqrt(-1)
extern number cI;

extern matrix cg_mu_nu;

/// \brief Get a random number uniformly distributed between zero and 1.
/// \return A random number
double randomDouble();

/// \brief Get alpha_electroweak for a given energy scale.
/// \return alpha_electroweak
double alpha_QED(double scale);

/// Return one of the Dirac gamma matrices
matrix gamma(int mu);

/// Calculates the matrix product a*b
matrix mul44_44(const matrix &a, const matrix &b);
/// Calculates the vector-matrix product
matrix mul14_44(const matrix &a, const matrix &b);
/// Calculates the vector dot-product
number mul14_41(const matrix &a, const matrix &b);
/// Calculates a matrix-vector product
matrix mul44_41(const matrix &a, const matrix &b);
/// Scales a matrix by a constant
matrix muln_44(number n, const matrix &b);

/// Returns the identity 4x4 matrix scaled by a number
matrix id_44(number n);

/// Initialises helper constants
void init();

#endif

