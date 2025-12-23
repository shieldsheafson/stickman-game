#include "box.h"

Collision Box::Collides(const Box& other) const {
  return ::Collides(GetMin(), GetMax(), other.GetMin(), other.GetMax());
}

Box& Box::operator+=(const Float2& rhs) {
  rect.x += rhs.x;
  rect.y += rhs.y;
  return *this;
}

Box& Box::operator-=(const Float2& rhs) {
  rect.x -= rhs.x;
  rect.y -= rhs.y;
  return *this;
}

Box operator+(const Box& lhs, const Float2& rhs) {
  return Box(lhs.GetMin() + rhs, lhs.GetWidth(), lhs.GetHeight());
}

Box operator-(const Box& lhs, const Float2& rhs) {
  return Box(lhs.GetMin() - rhs, lhs.GetWidth(), lhs.GetHeight());
}

std::ostream& operator<<(std::ostream& os, const Box& t) {
  os << "Left: " << t.GetLeft() << " Top: " << t.GetTop() << " Width: " << t.GetWidth() << " Height: " << t.GetHeight();
  return os;
}

std::istream& operator>>(std::istream& is, Box& t) {
  float x, y, w, h = 0.0f;
  is >> x >> y >> w >> h;
  t = Box(x, y, w, h);
  return is;
}