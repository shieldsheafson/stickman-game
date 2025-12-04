#pragma once

#include "float2.h"
#include "level.h"
#include "player.h"
#include "box.h"
#include "attack.h"

#include <list>
#include <memory>
#include <vector>

#include <SDL3/SDL.h>

class Game {
  private:
    int a = 0;
    Player mPlayer;
    Float2 mCamera;

    float mWindowWidth;
    float mWindowHeight;

    int mCurrentLevelIndex = 0;
    std::vector<Level> mLevels;
    std::list<std::unique_ptr<Attack> > mAttacks;

    void UpdateMovement(const bool *keystate, float deltaTime);
    void UpdateCollisions();

    void RenderCurrentLevel(SDL_Renderer *renderer) const;
    void RenderPlayer(SDL_Renderer *renderer) const;

  public:
    Game() = delete;
    Game(SDL_Texture* texture, const std::vector<Level>& levels, float windowWidth, float windowHeight)
      : mPlayer(texture, Float2(0,0), 50, 99), mLevels(levels), mWindowHeight(windowHeight), mWindowWidth(windowWidth) {
        mCamera = mPlayer.GetPosition();
      }

    const Player& GetPlayer() const { return mPlayer; }
    const Level& GetLevel() const { return mLevels[mCurrentLevelIndex]; }

    void Update(const bool *keystate, float deltaTime);
    void Render(SDL_Renderer *renderer);

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    Game(Game&&) noexcept = default;
    Game& operator=(Game&&) noexcept = default;
    ~Game() = default;
};