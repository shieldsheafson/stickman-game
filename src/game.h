#pragma once

#include "float2.h"
#include "level.h"
#include "player.h"
#include "terrain.h"

#include <vector>

#include <SDL3/SDL_scancode.h>

class Game {
  private:
    Player mPlayer;

    Level mCurrentLevel;
    std::vector<Level> mLevels;

    void MovementUpdate(const bool *keystate, float deltaTime);
    void CollisionsUpdate();

  public:
    Game() {}
    Game(const Player& player, const std::vector<Level>& levels)
      : mPlayer(player), mLevels(levels) {
        mCurrentLevel = levels.at(0);
      }

    const Player& GetPlayer() { return mPlayer; }
    const Level& GetLevel() { return mCurrentLevel; }

    void Update(const bool *keystate, float deltaTime);
};