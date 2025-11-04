#pragma once

#include "float2.h"
#include "level.h"
#include "player.h"
#include "terrain.h"
#include "utils.h"

#include <vector>

#include <SDL3/SDL.h>

class Game {
  private:
    Player mPlayer;

    Level mCurrentLevel;
    std::vector<Level> mLevels;

    void MovementUpdate(const bool *keystate, float deltaTime);
    void CollisionsUpdate();

    void RenderCurrentLevel(SDL_Renderer *renderer) const;
    void RenderPlayer(SDL_Renderer *renderer) const;

  public:
    Game() {}
    Game(const Player& player, const std::vector<Level>& levels)
      : mPlayer(player), mLevels(levels) {
        mCurrentLevel = levels.at(0);
      }

    const Player& GetPlayer() const { return mPlayer; }
    const Level& GetLevel() const { return mCurrentLevel; }

    void Update(const bool *keystate, float deltaTime);
    void Render(SDL_Renderer *renderer) const;
};