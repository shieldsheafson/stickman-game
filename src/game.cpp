#include "game.h"

void Game::UpdateCollisions() {
  mPlayer.SetOnGround(false);
  mPlayer.SetOnLeftWall(false);
  mPlayer.SetOnRightWall(false);
  mPlayer.SetForceDuck(false);

  for (const Box& terrain : mLevels[mCurrentLevelIndex].mTerrain) {
    Float2 playerMin = Float2(mPlayer.GetLeft(), mPlayer.GetTop());
    Float2 playerMax = Float2(mPlayer.GetRight(), mPlayer.GetBottom());
    Collision collision = Collides(playerMin, playerMax, terrain.GetMin(), terrain.GetMax());
    switch (collision) {
      case Collision::NONE:
        break;

      case Collision::GROUND:
        mPlayer.SetBottom(terrain.GetTop());
        mPlayer.SetVelocityY(0);
        mPlayer.SetOnGround(true);
        break;

      case Collision::CEILING:
        mPlayer.SetTop(terrain.GetBottom());
        mPlayer.SetVelocityY(0);
        mPlayer.SetOnGround(false);
        break;

      case Collision::LEFTWALL:
        mPlayer.SetLeft(terrain.GetRight());
        mPlayer.SetVelocityX(0);
        mPlayer.SetOnLeftWall(true);
        break;

      case Collision::RIGHTWALL:
        mPlayer.SetRight(terrain.GetLeft());
        mPlayer.SetVelocityX(0);
        mPlayer.SetOnRightWall(true);
        break;
    }

    Float2 baseTopLeft = Float2(mPlayer.GetLeft(), mPlayer.GetBaseTop());
    Float2 baseTopRight = Float2(mPlayer.GetRight(), mPlayer.GetBaseTop());
    if (terrain.Contains(baseTopLeft) || terrain.Contains(baseTopRight)) {
      mPlayer.SetForceDuck(true);
    }
  }
}

void Game::UpdateMovement(const Inputs& inputs, float deltaTime) {
  std::unique_ptr<Attack> currentPlayerAttack = mPlayer.Update(inputs, deltaTime);
  if (currentPlayerAttack) {
    mAttacks.push_back(std::move(currentPlayerAttack));
  }
}

void Game::Update(const Inputs& inputs, float deltaTime) {

  UpdateMovement(inputs, deltaTime);
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

void Game::Render(SDL_Renderer *renderer) {
  RenderCurrentLevel(renderer);
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
