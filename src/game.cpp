#include "game.h"

void Game::CollisionsUpdate() {
  for (const Terrain& terrain : mCurrentLevel.mTerrain) {
    Float2 playerMin = Float2(mPlayer.GetLeft(), mPlayer.GetBottom());
    Float2 playerMax = Float2(mPlayer.GetRight(), mPlayer.GetTop());
    Collision collision = Collides(playerMin, playerMax, terrain.GetMin(), terrain.GetMax());

    switch (collision) {
      case Collision::NONE:
        mPlayer.SetOnGround(false);
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
        break;

      case Collision::RIGHTWALL:
        mPlayer.SetRight(terrain.GetLeft());
        mPlayer.SetVelocityX(0);
        break;
    }
  }
}

void Game::MovementUpdate(const bool *keystate, float deltaTime) {
  mPlayer.SetMovingLeft(keystate[SDL_SCANCODE_LEFT]);
  mPlayer.SetMovingRight(keystate[SDL_SCANCODE_RIGHT]);
  if (keystate[SDL_SCANCODE_Z]) {
    mPlayer.Jump();
  }

  mPlayer.Update(deltaTime);
}

void Game::Update(const bool *keystate, float deltaTime) {
  MovementUpdate(keystate, deltaTime);
  CollisionsUpdate();

  // hack to keep player on the screen
  if (mPlayer.GetPosition().y > 340 || mPlayer.GetPosition().y < -340) {
    mPlayer.GetPosition() = Float2(0,0);
  }
}