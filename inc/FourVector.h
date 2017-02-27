/**
 * \class FourVector
 *
 * \ingroup simpleMC
 *
 * \brief Implement vectors in Minkowski metrics.
 *
 * This implements the basic four-vector operations needed.
 *
 */

#ifndef FOURVECTOR_H
#define FOURVECTOR_H

#include <iostream>

class ThreeVector;

class FourVector {
  public:

    /// Constructor from space-time coordinates
    FourVector(double px = 0, double py = 0, double pz = 0, double E = 0);

    /// Destructor
    virtual ~FourVector();

    /// copy constructor
    FourVector(const FourVector &other);

    /// \brief Get Euclidean vector with space components only
    /// \return A 3-vector
    ThreeVector Vect() const;

    /// \brief Calculate inner product between this vector and the parameter
    /// \param other Another 4-vector
    /// \return The value of the inner product
    double Dot(const FourVector &other) const;

    /// Mass getter
    double M() const;

    /// Mass squared getter
    double M2() const;

    /// Rapidity getter
    double Rapidity() const;

    /// Transverse momentum getter
    double Perp() const;

    /// Getter of the magnitude of the 3-momentum
    double P() const;

    /// X coordinate getter
    double Px() const;

    /// Y coordinate getter
    double Py() const;

    /// Z coordinate getter
    double Pz() const;

    /// T coordinate getter
    double E() const;

    /// \brief Sets the four-momentum.
    /// \param px The x coordinate.
    /// \param py The y coordinate.
    /// \param pz The z coordinate.
    /// \param e  The t coordinate.
    void SetPxPyPzE(double px, double py, double pz, double e);

    /// \brief Sets the four-momentum.
    /// \param pt  Transverse momentum.
    /// \param eta Pseudo-rapidity.
    /// \param phi Azimuth.
    /// \param m   Mass.
    void SetPtEtaPhiM(double pt, double eta, double phi, double m);

    /// \brief Adds two four-momenta.
    /// \param other Another 4-vector.
    /// \return The sum.
    FourVector operator +(const FourVector &other) const;

    /// \brief Adds another four-momentum to this.
    /// \param other Another 4-vector.
    /// \return This vector.
    FourVector &operator +=(const FourVector &other);

    /// \brief Subtracts a four-momenta.
    /// \param other Another 4-vector.
    /// \return The subtraction.
    FourVector operator -(const FourVector &other) const;

    /// \brief Subtracts another four-momentum from this.
    /// \param other Another 4-vector.
    /// \return This vector.
    FourVector &operator -=(const FourVector &other);

    /// \brief Scales a four-momenta.
    /// \param other A scalar.
    /// \return The result.
    FourVector operator *(const double f) const;

  private:
    double _px; ///< X coordinate
    double _py; ///< Y coordinate
    double _pz; ///< Z coordinate
    double _E;  ///< T coordinate

};

/// \brief Negates a four-momentum.
/// \return The negated four-momentum.
FourVector operator -(const FourVector &x);

/// \brief Dumps 4-vector in the output stream.
/// \return The output stream.
std::ostream &operator <<(std::ostream &o, const FourVector &v);

#endif

