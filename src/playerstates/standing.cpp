#include "player.h"

void Player::Standing::UpdateHorizontalVelocity(float deltaTime) {
  if (mPlayer.mMovingLeft && mPlayer.mMovingRight) {
    return;
  }

  float deltaSpeed = mPlayer.mHorizontalAcceleration * deltaTime;

  if (mPlayer.mMovingLeft) {
    if (mPlayer.OpposingVelocity()) { mPlayer.mVelocity.x = 0; }
    mPlayer.mVelocity.x = std::max(-mPlayer.mMaxHorizontalSpeed, mPlayer.mVelocity.x - deltaSpeed);
  } else if (mPlayer.mMovingRight) {
    if (mPlayer.OpposingVelocity()) { mPlayer.mVelocity.x = 0; }
    mPlayer.mVelocity.x = std::min(mPlayer.mMaxHorizontalSpeed, mPlayer.mVelocity.x + deltaSpeed);
  } else {
    mPlayer.ApplyFriction(deltaTime);
  }
}

void Player::Standing::UpdateVerticalVelocity(float deltaTime) {
  mPlayer.mVelocity.y = 0.0f;
}

void Player::Standing::ChangeState() {
  std::cout << StateName() << std::endl;
  if (!mPlayer.mOnGround) {
    mPlayer.ChangeStateTo<Falling>();
    return;
  }
  
  if (mPlayer.mJumping) {
    mPlayer.ChangeStateTo<Jumping>();
    return;
  }

  if (mPlayer.mDucking) {
    if (mPlayer.mVelocity.x >= mPlayer.mMaxHorizontalSpeed * 0.8) {
      mPlayer.ChangeStateTo<GroundSliding>();
    } else {
      mPlayer.ChangeStateTo<Ducking>();
    }
    return;
  }
}
