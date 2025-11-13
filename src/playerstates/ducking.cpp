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
  const Inputs& inputs = mPlayer.mInputs.GetInputs();
  if (inputs.mLeftKeyPressed && inputs.mRightKeyPressed) {
    return;
  }

  float deltaSpeed = mPlayer.mHorizontalAcceleration * mPlayer.mDuckSpeedModifier * deltaTime;

  if (inputs.mLeftKeyPressed) {
    mPlayer.mVelocity.x = std::max(-mPlayer.mMaxHorizontalSpeed * mPlayer.mDuckSpeedModifier, mPlayer.mVelocity.x - deltaSpeed);
  } else if (inputs.mRightKeyPressed) {
    mPlayer.mVelocity.x = std::min(mPlayer.mMaxHorizontalSpeed * mPlayer.mDuckSpeedModifier, mPlayer.mVelocity.x + deltaSpeed);
  } else {
    mPlayer.ApplyFriction(deltaTime);
  }
}

void Player::Ducking::UpdateVerticalVelocity(float deltaTime) {
  mPlayer.mVelocity.y = 0.0f;
}

void Player::Ducking::ChangeState() {
  const Inputs& inputs = mPlayer.mInputs.GetInputs();
  if (!mPlayer.mOnGround) {
    mPlayer.ChangeStateTo<Falling>();
    return;
  } 
  
  if (inputs.mJumpKeyPressed) {
    mPlayer.ChangeStateTo<Jumping>();
    return;
  }

  if (!inputs.mDownKeyPressed) {
    mPlayer.ChangeStateTo<Standing>();
  }
}
