#pragma once

#include <cmath>
#include <iostream>


struct Float2 {
  float x;
  float y;

  Float2(): x(0), y(0) {}
  Float2(float x_, float y_): x(x_), y(y_) {}

  float Dot(const Float2& other) { return x * other.x + y * other.y; }
  Float2 Perpendicular() { return Float2(y, x * -1); }

  // arithmetic operators with other Float2---------------------
  Float2& operator+=(const Float2& rhs);
  Float2& operator-=(const Float2& rhs);

  // arithmetic operators with scalar------------------------
  Float2& operator*=(float rhs);
  Float2& operator/=(float rhs);
};

// equality----------------------------------------------------
bool operator==(const Float2& lhs, const Float2& rhs);
bool operator!=(const Float2& lhs, const Float2& rhs);

// arithmetic operators with other Float2-----------------------
Float2 operator+(const Float2& lhs, const Float2& rhs);
Float2 operator-(const Float2& lhs, const Float2& rhs);

// arithmetic operators with scalar--------------------------
Float2 operator*(const Float2& lhs, float rhs);
Float2 operator/(const Float2& lhs, float rhs);

// input/output-------------------------------------------------
std::ostream& operator<<(std::ostream& os, const Float2& it);
std::istream& operator>>(std::istream& is, Float2& p);
