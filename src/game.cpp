#include "game.h"

void Game::CollisionsUpdate() {
  bool collided = false;
  for (const Terrain& terrain : mCurrentLevel.mTerrain) {
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
        collided = true;
        break;

      case Collision::CEILING:
        mPlayer.SetTop(terrain.GetBottom());
        mPlayer.SetVelocityY(0);
        mPlayer.SetOnGround(false);
        collided = true;
        break;

      case Collision::LEFTWALL:
        mPlayer.SetRight(terrain.GetLeft());
        mPlayer.SetVelocityX(0);
        mPlayer.SetOnLeftWall(true);
        collided = true;
        break;

      case Collision::RIGHTWALL:
        mPlayer.SetLeft(terrain.GetRight());
        mPlayer.SetVelocityX(0);
        mPlayer.SetOnRightWall(true);
        collided = true;
        break;
    }
  }

  if (!collided) {
    mPlayer.SetOnGround(false);
  }
}

void Game::MovementUpdate(const bool *keystate, float deltaTime) {
  mPlayer.SetMovingLeft(keystate[SDL_SCANCODE_LEFT]);
  mPlayer.SetMovingRight(keystate[SDL_SCANCODE_RIGHT]);
  if (keystate[SDL_SCANCODE_Z]) {
    mPlayer.SetJumping(true);
  } else {
    mPlayer.SetJumping(false);
  }

  mPlayer.Update(deltaTime);
}

void Game::Update(const bool *keystate, float deltaTime) {
  MovementUpdate(keystate, deltaTime);
  CollisionsUpdate();

  // hack to keep player on the screen
  if (mPlayer.GetPosition().y > mCurrentLevel.mHeight) {
    mPlayer.GetPosition() = Float2(0,0);
  }

  mCamera = mPlayer.GetPosition() - Float2(mWindowWidth/2 - mPlayer.GetWidth() / 2, mWindowHeight/2 - mPlayer.GetHeight() / 2);
}

void Game::RenderCurrentLevel(SDL_Renderer *renderer) const {
  for (const Terrain& terrain : mCurrentLevel.mTerrain) {
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

void Game::Render(SDL_Renderer *renderer) const {
  RenderCurrentLevel(renderer);
  RenderPlayer(renderer);
  
  SDL_RenderPresent(renderer);  // actually put it on the screen
}