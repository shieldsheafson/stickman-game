#include "terrain.h"

std::ostream& operator<<(std::ostream& os, const Terrain& t) {
  os << t.GetLeft() << " " << t.GetTop() << " " << t.GetWidth() << " " << t.GetHeight();
  return os;
}

std::istream& operator>>(std::istream& is, Terrain& t) {
  float x, y, w, h = 0.0f;
  is >> x >> y >> w >> h;
  t = Terrain(x, y, w, h);
  return is;
}