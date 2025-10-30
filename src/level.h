#pragma once

#include "float2.h"
#include "player.h"
#include "terrain.h"

#include <vector>

class Level {
  private:
    std::vector<Terrain> mTerrain;

  public:
    Level() {}

    void Collision(Player& p) {
      for (const Terrain& terrain : mTerrain) {
        
      }
    }
};