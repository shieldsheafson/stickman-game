#include "player.h"

void Player::GroundSliding::OnEnter() {
  mPlayer.mPosition.y += mPlayer.mTextureHeight / 2;
  mPlayer.mCurrentHeight = mPlayer.mTextureHeight / 2;
}

void Player::GroundSliding::OnExit() {
  mPlayer.mPosition.y -= mPlayer.mTextureHeight / 2;
  mPlayer.mCurrentHeight = mPlayer.mTextureHeight;
}

void Player::GroundSliding::UpdateHorizontalVelocity(float deltaTime) {
  if (mPlayer.mMovingLeft && mPlayer.mMovingRight) {
    return;
  }

  float deltaSpeed = mPlayer.mHorizontalAcceleration * deltaTime;

  if (mPlayer.mMovingLeft) {
    mPlayer.mVelocity.x = std::max(-mPlayer.mMaxHorizontalSpeed, mPlayer.mVelocity.x - deltaSpeed);
  } else if (mPlayer.mMovingRight) {
    mPlayer.mVelocity.x = std::min(mPlayer.mMaxHorizontalSpeed, mPlayer.mVelocity.x + deltaSpeed);
  } else {
    mPlayer.ApplyFriction(deltaTime);
  }

  mSlideTime += deltaTime;
}

void Player::GroundSliding::UpdateVerticalVelocity(float deltaTime) {
  mPlayer.mVelocity.y = 0.0f;
}

void Player::GroundSliding::ChangeState() {
  std::cout << StateName() << std::endl;
  if (!mPlayer.mOnGround) {
    mPlayer.ChangeStateTo<Falling>();
    return;
  } 
  
  if (mPlayer.mJumping) {
    mPlayer.ChangeStateTo<Jumping>();
    return;
  }

  if (mSlideTime > mPlayer.mMaxSlideTime || mPlayer.mVelocity.x == 0 || mPlayer.OpposingVelocity()) {
    if (mPlayer.mDucking) {
      mPlayer.ChangeStateTo<Ducking>();
    } else {
      mPlayer.ChangeStateTo<Standing>();
    }
    return;
  }
}
