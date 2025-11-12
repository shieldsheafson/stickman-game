#include "player.h"

void Player::Ducking::OnEnter() {
  mPlayer.mPosition.y += mPlayer.mTextureHeight / 2;
  mPlayer.mCurrentHeight = mPlayer.mTextureHeight / 2;
}

void Player::Ducking::OnExit() {
  mPlayer.mPosition.y -= mPlayer.mTextureHeight / 2;
  mPlayer.mCurrentHeight = mPlayer.mTextureHeight;
}

void Player::Ducking::UpdateHorizontalVelocity(float deltaTime) {
  if (mPlayer.mMovingLeft && mPlayer.mMovingRight) {
    return;
  }

  float duckSpeedModifier = 0.25;

  float deltaSpeed = mPlayer.mHorizontalAcceleration * duckSpeedModifier * deltaTime;

  if (mPlayer.mMovingLeft) {
    mPlayer.mVelocity.x = std::max(-mPlayer.mMaxHorizontalSpeed * duckSpeedModifier, mPlayer.mVelocity.x - deltaSpeed);
  } else if (mPlayer.mMovingRight) {
    mPlayer.mVelocity.x = std::min(mPlayer.mMaxHorizontalSpeed * duckSpeedModifier, mPlayer.mVelocity.x + deltaSpeed);
  } else {
    mPlayer.ApplyFriction(deltaTime);
  }
}

void Player::Ducking::UpdateVerticalVelocity(float deltaTime) {
  mPlayer.mVelocity.y = 0.0f;
}

void Player::Ducking::ChangeState() {
  std::cout << StateName() << std::endl;
  if (!mPlayer.mOnGround) {
    mPlayer.ChangeStateTo<Falling>();
    return;
  } 
  
  if (mPlayer.mJumping) {
    mPlayer.ChangeStateTo<Jumping>();
    return;
  }

  if (!mPlayer.mDucking) {
    mPlayer.ChangeStateTo<Standing>();
  }
}
