#include "FourVector.h"
#include "ThreeVector.h"
#include <cmath>
#include <algorithm>

FourVector::FourVector(double px, double py, double pz, double E) {
  _px = px;
  _py = py;
  _pz = pz;
  _E = E;
}

FourVector::~FourVector() {
}

FourVector::FourVector(const FourVector &other) {
  _px = other._px;
  _py = other._py;
  _pz = other._pz;
  _E = other._E;
}

ThreeVector FourVector::Vect() const {
  ThreeVector v(_px, _py, _pz);
  return v;
}

double FourVector::Dot(const FourVector &other) const {
  return - _px*other._px - _py*other._py - _pz*other._pz + _E*other._E;
}

double FourVector::M() const {
  double m2 = M2();
  if (m2 < 0)
    return std::sqrt(-m2);
  return std::sqrt(m2);
}

double FourVector::M2() const {
  return std::pow(_E, 2) - std::pow(_px, 2) - std::pow(_py, 2) - std::pow(_pz, 2);
}

double FourVector::Rapidity() const {
  return 0.5*std::log( (_E+_pz)/(_E-_pz) );
}

double FourVector::P() const {
  return std::sqrt(_px*_px + _py*_py + _pz*_pz);
}

double FourVector::Perp() const {
  return std::sqrt(_px*_px + _py*_py);
}

double FourVector::Px() const {
  return _px;
}

double FourVector::Py() const {
  return _py;
}

double FourVector::Pz() const {
  return _pz;
}

double FourVector::E() const {
  return _E;
}

void FourVector::SetPxPyPzE(double px, double py, double pz, double E) {
  _px = px;
  _py = py;
  _pz = pz;
  _E = E;
}

void FourVector::SetPtEtaPhiM(double pt, double eta, double phi, double m) {
  pt = std::fabs(pt);
  _px = pt*std::cos(phi);
  _py = pt*std::sin(phi);
  _pz = pt*std::sinh(eta);
  if (m < 0)
    _E = std::max<double>(-std::sqrt(std::pow(m, 2) + std::pow(_px, 2) + std::pow(_py, 2) + std::pow(_pz, 2)), 0);
  else
    _E = std::sqrt(std::pow(m, 2) + std::pow(_px, 2) + std::pow(_py, 2) + std::pow(_pz, 2));
}

FourVector FourVector::operator +(const FourVector &other) const {
  return FourVector(_px+other._px, _py+other._py, _pz+other._pz, _E+other._E);
}

FourVector &FourVector::operator +=(const FourVector &other) {
  _px += other._px;
  _py += other._py;
  _pz += other._pz;
  _E  += other._E;
  return *this;
}

FourVector FourVector::operator -(const FourVector &other) const {
  return FourVector(_px-other._px, _py-other._py, _pz-other._pz, _E-other._E);
}

FourVector &FourVector::operator -=(const FourVector &other) {
  _px -= other._px;
  _py -= other._py;
  _pz -= other._pz;
  _E  -= other._E;
  return *this;
}

FourVector FourVector::operator *(const double f) const {
  return FourVector(_px*f, _py*f, _pz*f, _E*f);
}


FourVector operator -(const FourVector &x) {
  return FourVector(-x.Px(), -x.Py(), -x.Pz(), -x.E());
}

std::ostream &operator <<(std::ostream &o, const FourVector &v) {
  o << "(" << v.Px() << ", " << v.Py() << ", " << v.Pz() << ", " << v.E() << ")";
  return o;
}

