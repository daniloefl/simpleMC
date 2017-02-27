#include "ThreeVector.h"

ThreeVector::ThreeVector(double px, double py, double pz) {
  _px = px;
  _py = py;
  _pz = pz;
}

ThreeVector::~ThreeVector() {
}

ThreeVector::ThreeVector(const ThreeVector &other) {
  _px = other._px;
  _py = other._py;
  _pz = other._pz;
}

double ThreeVector::Dot(const ThreeVector &other) const {
  return _px*other._px + _py*other._py + _pz*other._pz;
}

double ThreeVector::Px() const {
  return _px;
}

double ThreeVector::Py() const {
  return _py;
}

double ThreeVector::Pz() const {
  return _pz;
}

