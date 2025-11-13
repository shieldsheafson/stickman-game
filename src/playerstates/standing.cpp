#include "player.h"

void Player::Standing::UpdateHorizontalVelocity(float deltaTime) {
  const Inputs& inputs = mPlayer.mInputs.GetInputs();
  if (inputs.mLeftKeyPressed && inputs.mRightKeyPressed) {
    return;
  }

  float deltaSpeed = mPlayer.mHorizontalAcceleration * deltaTime;

  if (inputs.mLeftKeyPressed) {
    if (mPlayer.OpposingVelocity()) { mPlayer.mVelocity.x = 0; }
    mPlayer.mVelocity.x = std::max(-mPlayer.mMaxHorizontalSpeed, mPlayer.mVelocity.x - deltaSpeed);
  } else if (inputs.mRightKeyPressed) {
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
  const Inputs& inputs = mPlayer.mInputs.GetInputs();
  if (!mPlayer.mOnGround) {
    mPlayer.ChangeStateTo<Falling>();
    return;
  }
  
  if (inputs.mJumpKeyPressed) {
    mPlayer.ChangeStateTo<Jumping>();
    return;
  }

  if (inputs.mDownKeyPressed) {
    if (mPlayer.mVelocity.x >= mPlayer.mMaxHorizontalSpeed * mPlayer.mPercentOfMaxSpeedRequiredToSlide) {
      mPlayer.ChangeStateTo<GroundSliding>();
    } else {
      mPlayer.ChangeStateTo<Ducking>();
    }
    return;
  }
}
