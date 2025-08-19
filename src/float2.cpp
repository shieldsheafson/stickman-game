#include "Float2.h"

// arithmetic operators with other Float2---------------------
Float2& Float2::operator+=(const Float2& rhs) {
  this->x += rhs.x;
  this->y += rhs.y;
  return *this;
}
Float2& Float2::operator-=(const Float2& rhs) {
  this->x -= rhs.x;
  this->y -= rhs.y;
  return *this;
}
//-----------------------------------------------------------



// arithmetic operators with scalar------------------------
Float2& Float2::operator*=(float rhs) {
  this->x *= rhs;
  this->y *= rhs;
  return *this;
}
Float2& Float2::operator/=(float rhs) {
  this->x /= rhs;
  this->y /= rhs;
  return *this;
}
//-----------------------------------------------------------



// equality----------------------------------------------------
bool operator==(const Float2& lhs, const Float2& rhs) {
  return lhs.x == rhs.x && lhs.y == rhs.y;
}
bool operator!=(const Float2& lhs, const Float2& rhs) {
  return !(lhs == rhs);
}
//-------------------------------------------------------------



// arithmetic operators with other Float2-----------------------
Float2 operator+(const Float2& lhs, const Float2& rhs) {
  return Float2(lhs.x + rhs.x, lhs.y + rhs.y);
}
Float2 operator-(const Float2& lhs, const Float2& rhs) {
  return Float2(lhs.x - rhs.x, lhs.y - rhs.y);
}
//-------------------------------------------------------------



// arithmetic operators with scalar--------------------------
Float2 operator*(const Float2& lhs, float rhs) {
  return Float2(lhs.x * rhs, lhs.y * rhs);
}
Float2 operator/(const Float2& lhs, float rhs) {
  return Float2(lhs.x / rhs, lhs.y / rhs);
}
//--------------------------------------------------------------


// input/output-------------------------------------------------
std::ostream& operator<<(std::ostream& os, const Float2& it) {
  os << it.x << " " << it.y;
  return os;
}
std::istream& operator>>(std::istream& is, Float2& p) {
  is >> p.x  >> p.y;
  return is;
}
//--------------------------------------------------------------