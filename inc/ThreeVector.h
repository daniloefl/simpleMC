/**
 * \class ThreeVector
 * \ingroup simpleMC
 * \brief This class implements the basic operations needed for 3D vectors.
 *
 */

#ifndef THREEVECTOR_H
#define THREEVECTOR_H

class ThreeVector {
  public:

    /// Constructor that builds object from Euclidean coordinates
    ThreeVector(double px = 0, double py = 0, double pz = 0);

    /// Destructor
    virtual ~ThreeVector();

    /// Copy constructor
    ThreeVector(const ThreeVector &other);

    /// \brief Calculates the scalar product between 3-vectors.
    /// \param other another vector
    /// \return The result of the dot product between the current vector and other.
    double Dot(const ThreeVector &other) const;

    /// X getter
    double Px() const;

    /// Y getter
    double Py() const;

    /// Z getter
    double Pz() const;

  private:

    double _px;  ///< X coordinate
    double _py;  ///< Y coordinate
    double _pz;  ///< Z coordinate

};

#endif

