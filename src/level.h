#pragma once

#include "collision.h"
#include "float2.h"
#include "player.h"
#include "box.h"

#include <iostream>
#include <vector>

struct Level {
  float mHeight;
  float mWidth;
  std::vector<Box> mTerrain;

  Level() {}
};

std::ostream& operator<<(std::ostream& os, const Level& l);
std::istream& operator>>(std::istream& is, Level& l);