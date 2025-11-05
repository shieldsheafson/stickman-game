#pragma once

#include "collision.h"
#include "float2.h"
#include "player.h"
#include "terrain.h"

#include <iostream>
#include <vector>

struct Level {
  std::vector<Terrain> mTerrain;

  Level() {}
  Level(const std::vector<Terrain>& terrain): mTerrain(terrain) {}
};

std::ostream& operator<<(std::ostream& os, const Level& l);
std::istream& operator>>(std::istream& is, Level& l);