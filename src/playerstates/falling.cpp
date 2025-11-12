#include "player.h"

void Player::Falling::UpdateHorizontalVelocity(float deltaTime) {
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
}

void Player::Falling::UpdateVerticalVelocity(float deltaTime) {
  float deltaSpeed = mPlayer.mHorizontalAcceleration * deltaTime;

  mPlayer.mVelocity.y = std::min(mPlayer.mTerminalVelocity, 
                                 mPlayer.mVelocity.y + mPlayer.mGravity * deltaTime);
}

void Player::Falling::ChangeState() {
  std::cout << StateName() << std::endl;
  if (mPlayer.mOnGround) {
    mPlayer.ChangeStateTo<Standing>();
  } else if (mPlayer.mOnLeftWall && mPlayer.mMovingLeft) {
    mPlayer.ChangeStateTo<WallSliding>();
  } else if (mPlayer.mOnRightWall && mPlayer.mMovingRight) {
    mPlayer.ChangeStateTo<WallSliding>();
  }
}
