#pragma once

#include "float2.h"
#include "level.h"
#include "player.h"
#include "box.h"
#include "attack.h"
#include "inputs.h"
#include "enemy.h"
#include "entity.h"

#include <list>
#include <memory>
#include <vector>

#include <SDL3/SDL.h>

class Game {
  private:
    Player mPlayer;
    std::list<std::unique_ptr<Enemy> > mEnemies;
    Float2 mCamera;

    float mWindowWidth;
    float mWindowHeight;

    int mCurrentLevelIndex = 1;
    std::vector<Level> mLevels;
    std::list<std::unique_ptr<Attack> > mPlayerAttacks;
    std::list<std::unique_ptr<Attack> > mEnemyAttacks;

    void UpdateMovement(const Inputs& inputs, float deltaTime);
    void UpdateEnemies(float deltaTime);
    void UpdateCollisions();
    void CheckCollision(const Box& terrain, Entity* entity);
    void UpdateAttacks(float deltaTime);

    void RenderCurrentLevel(SDL_Renderer *renderer) const;
    void RenderPlayer(SDL_Renderer *renderer) const;
    void RenderEnemies(SDL_Renderer *renderer) const;

  public:
    Game() = delete;
    Game(SDL_Texture* texture, const std::vector<Level>& levels, float windowWidth, float windowHeight)
      : mPlayer(texture, Float2(0,0), 10, 50, 99), mLevels(levels), mWindowHeight(windowHeight), mWindowWidth(windowWidth) {
        Float2 spawn = mLevels.at(mCurrentLevelIndex).mSpawn;
        mPlayer.SetPosition(Float2(spawn.x, spawn.y - mPlayer.GetHeight()));
        mCamera = mPlayer.GetPosition();
        mEnemies.push_back(std::make_unique<Enemy>(texture, Float2(1000,1700), 10, 50, 99));
        UpdateCollisions();
      }

    const Player& GetPlayer() const { return mPlayer; }
    const Level& GetLevel() const { return mLevels[mCurrentLevelIndex]; }

    void Update(const Inputs& inputs, float deltaTime);
    void Render(SDL_Renderer *renderer);

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    Game(Game&&) noexcept = default;
    Game& operator=(Game&&) noexcept = default;
    ~Game() = default;
};