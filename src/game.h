#pragma once

#include "float2.h"
#include "level.h"
#include "player.h"
#include "box.h"
#include "attack.h"
#include "inputs.h"
#include "enemy.h"
#include "entity.h"
#include "gamestate.h"
#include "enemyai.h"

#include <list>
#include <memory>
#include <vector>

#include <SDL3/SDL.h>

class Game {
  private:
    GameState mState;

    Player mPlayer;

    EnemyAI mEnemyAI;
    SDL_Texture* mEnemyTexture;
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
    void UpdateState();

    void RenderCurrentLevel(SDL_Renderer *renderer) const;
    void RenderPlayer(SDL_Renderer *renderer) const;
    void RenderEnemies(SDL_Renderer *renderer) const;

  public:
    Game() = delete;
    Game(SDL_Texture* playerTexture, SDL_Texture* enemyTexture, const std::vector<Level>& levels, float windowWidth, float windowHeight)
      : mPlayer(playerTexture, Float2(0,0), 5, 50, 99), mLevels(levels), mWindowHeight(windowHeight), mWindowWidth(windowWidth), mEnemyTexture(enemyTexture) {
        Float2 spawn = mLevels.at(mCurrentLevelIndex).mSpawn;
        mPlayer.SetPosition(Float2(spawn.x, spawn.y - mPlayer.GetHeight()));
        mCamera = mPlayer.GetPosition();
        mEnemies.push_back(std::make_unique<BasicEnemy>(mEnemyTexture, Float2(1000,1700), 2, 50, 99));
        UpdateCollisions();
        UpdateState();
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