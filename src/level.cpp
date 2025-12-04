#include "level.h"

std::ostream& operator<<(std::ostream& os, const Level& l) {
  for (const Box& t : l.mTerrain) {
    os << t << "\n";
  }

  return os;
}

std::istream& operator>>(std::istream& is, Level& l) {
  is >> l.mWidth >> l.mHeight;
  l.mTerrain.clear();
  float x, y, w, h;
  while (is >> x >> y >> w >> h) {
    l.mTerrain.push_back(Box(x, y, w, h));
  }

  return is;
}