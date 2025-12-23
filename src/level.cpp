#include "level.h"

Level::Level(std::istream& is): mWidth(0), mHeight(0) {
  is >> *this;
}

std::ostream& operator<<(std::ostream& os, const Level& l) {
  os << l.mWidth << " " << l.mHeight << "\n";
  os << l.mSpawn.x << " " << l.mSpawn.y << "\n";
  for (const Box& b : l.mTerrain) {
    os << b << "\n";
  }

  return os;
}

std::istream& operator>>(std::istream& is, Level& l) {
  is >> l.mWidth >> l.mHeight;
  is >> l.mSpawn;
  l.mTerrain.clear();
  float x, y, w, h;
  while (is >> x >> y >> w >> h) {
    l.mTerrain.push_back(Box(x, y, w, h));
  }

  return is;
}