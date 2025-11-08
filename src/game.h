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
    Float2 mCamera;

    float mWindowWidth;
    float mWindowHeight;

    Level mCurrentLevel;
    std::vector<Level> mLevels;

    void MovementUpdate(const bool *keystate, float deltaTime);
    void CollisionsUpdate();

    void RenderCurrentLevel(SDL_Renderer *renderer) const;
    void RenderPlayer(SDL_Renderer *renderer) const;

  public:
    Game() {}
    Game(const Player& player, const std::vector<Level>& levels, float windowWidth, float windowHeight)
      : mPlayer(player), mLevels(levels), mWindowHeight(windowHeight), mWindowWidth(windowWidth) {
        mCurrentLevel = levels.at(0);
        mCamera = mPlayer.GetPosition();
      }

    const Player& GetPlayer() const { return mPlayer; }
    const Level& GetLevel() const { return mCurrentLevel; }

    void Update(const bool *keystate, float deltaTime);
    void Render(SDL_Renderer *renderer) const;
};