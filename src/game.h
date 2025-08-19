#pragma once

#include "float2.h"
#include "player.h"
#include "terrain.h"

#include <vector>

#include <SDL3/SDL.h>

class Game {
  private:
    Player mPlayer;
    std::vector<Terrain> mTerrain;
};