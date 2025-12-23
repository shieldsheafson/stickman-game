#include "game.h"

void Game::CheckCollision(const Box& terrain, Entity* entity) {
  Float2 entityMin = Float2(entity->GetLeft(), entity->GetTop());
  Float2 entityMax = Float2(entity->GetRight(), entity->GetBottom());
  Collision collision = Collides(entityMin, entityMax, terrain.GetMin(), terrain.GetMax());
  switch (collision) {
    case Collision::NONE:
      break;

    case Collision::GROUND:
      entity->SetBottom(terrain.GetTop());
      entity->SetVelocityY(0);
      entity->SetOnGround(true);
      break;

    case Collision::CEILING:
      entity->SetTop(terrain.GetBottom());
      entity->SetVelocityY(0);
      entity->SetOnGround(false);
      break;

    case Collision::LEFTWALL:
      entity->SetLeft(terrain.GetRight());
      entity->SetVelocityX(0);
      entity->SetOnLeftWall(true);
      break;

    case Collision::RIGHTWALL:
      entity->SetRight(terrain.GetLeft());
      entity->SetVelocityX(0);
      entity->SetOnRightWall(true);
      break;
  }

  Float2 baseTopLeft = Float2(entity->GetLeft(), entity->GetBaseTop());
  Float2 baseTopRight = Float2(entity->GetRight(), entity->GetBaseTop());
  if (terrain.Contains(baseTopLeft) || terrain.Contains(baseTopRight)) {
    entity->SetForceDuck(true);
  }
}

void Game::UpdateCollisions() {
  mPlayer.SetOnGround(false);
  mPlayer.SetOnLeftWall(false);
  mPlayer.SetOnRightWall(false);
  mPlayer.SetForceDuck(false);

  for (auto& enemy : mEnemies) {
    enemy->SetOnGround(false);
    enemy->SetOnLeftWall(false);
    enemy->SetOnRightWall(false);
    enemy->SetForceDuck(false);
  }

  for (const Box& terrain : mLevels[mCurrentLevelIndex].mTerrain) {
    CheckCollision(terrain, &mPlayer);

    for (auto& enemy : mEnemies) {
      CheckCollision(terrain, enemy.get());
    }
  }
}

void Game::UpdateMovement(const Inputs& inputs, float deltaTime) {
  std::unique_ptr<Attack> currentPlayerAttack = mPlayer.Update(inputs, deltaTime);
  if (currentPlayerAttack) {
    mAttacks.push_back(std::move(currentPlayerAttack));
  }
}

void Game::UpdateEnemies(float deltaTime) {
  for (auto& enemy : mEnemies) {
    Inputs inputs;
    std::unique_ptr<Attack> currentEnemyAttack = enemy->Update(inputs, deltaTime);
    if (currentEnemyAttack) {
      mAttacks.push_back(std::move(currentEnemyAttack));
    }
  }
}

void Game::Update(const Inputs& inputs, float deltaTime) {

  UpdateMovement(inputs, deltaTime);
  UpdateEnemies(deltaTime);
  UpdateCollisions();

  // Update Attacks
  for (auto& attack : mAttacks) {
    attack->Update(deltaTime);
  }

  // hack to keep player on the screen
  if (mPlayer.GetPosition().y > mLevels[mCurrentLevelIndex].mHeight) {
    mPlayer.SetPosition(Float2(0,0));
  }

  mCamera = mPlayer.GetPositionForCamera() - Float2(mWindowWidth/2, mWindowHeight/2);
}

void Game::RenderCurrentLevel(SDL_Renderer *renderer) const {
  for (const Box& terrain : mLevels[mCurrentLevelIndex].mTerrain) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_FRect dst_rect = terrain.GetModifiedSDLRect(mCamera);
    SDL_RenderFillRect(renderer, &dst_rect);
  }
}

void Game::RenderPlayer(SDL_Renderer *renderer) const {
  SDL_FRect dst_rect{mPlayer.GetLeft() - mCamera.x,
                     mPlayer.GetTop() - mCamera.y, 
                     mPlayer.GetWidth(), 
                     mPlayer.GetHeight()};

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderTexture(renderer, mPlayer.GetTexture(), NULL, &dst_rect);
}

void Game::RenderEnemies(SDL_Renderer *renderer) const {
  for (const auto& enemy : mEnemies) {
    SDL_FRect dst_rect{enemy->GetLeft() - mCamera.x,
                     enemy->GetTop() - mCamera.y, 
                     enemy->GetWidth(), 
                     enemy->GetHeight()};

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderTexture(renderer, enemy->GetTexture(), NULL, &dst_rect);
  }
}

void Game::Render(SDL_Renderer *renderer) {
  RenderCurrentLevel(renderer);
  RenderEnemies(renderer);
  RenderPlayer(renderer);

  for (auto it = mAttacks.begin(); it != mAttacks.end(); ) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    const Box* currentFrame = (*it)->GetCurrentFrame();
    
    if (currentFrame) {
      SDL_FRect dst_rect = currentFrame->GetModifiedSDLRect(mCamera);
      SDL_RenderFillRect(renderer, &dst_rect);
      ++it;
    } else {
      it = mAttacks.erase(it);
    }
  }
  
  SDL_RenderPresent(renderer);  // actually put it on the screen
}
