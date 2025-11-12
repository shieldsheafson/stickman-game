#include "player.h"

void Player::Jumping::UpdateHorizontalVelocity(float deltaTime) {
  if (mPlayer.mMovingLeft && mPlayer.mMovingRight) {
    return;
  }

  float deltaSpeed = mPlayer.mHorizontalAcceleration * deltaTime;
  float deltaJumpSpeed = mPlayer.mJumpStrength * 25 * deltaTime;

  
  if (mPlayer.mOnLeftWall && !mPlayer.mOnGround) {
    mPlayer.mVelocity.x = deltaJumpSpeed;
  } else if (mPlayer.mOnRightWall && !mPlayer.mOnGround) {
    mPlayer.mVelocity.x = -deltaJumpSpeed;
  } else if (mPlayer.mMovingLeft) {
    mPlayer.mVelocity.x = std::max(-mPlayer.mMaxHorizontalSpeed, mPlayer.mVelocity.x - deltaSpeed);
  } else if (mPlayer.mMovingRight) {
    mPlayer.mVelocity.x = std::min(mPlayer.mMaxHorizontalSpeed, mPlayer.mVelocity.x + deltaSpeed);
  } else {
    mPlayer.ApplyFriction(deltaTime);
  }
}

void Player::Jumping::UpdateVerticalVelocity(float deltaTime) {
  float deltaSpeed = mPlayer.mHorizontalAcceleration * deltaTime;
  float gravityToApply = mPlayer.mGravity;

  if (mPlayer.mOnGround) {
    mPlayer.mVelocity.y = -mPlayer.mJumpStrength;
    mPlayer.mOnGround = false;
  } else if (mPlayer.mOnLeftWall || mPlayer.mOnRightWall) {
    mPlayer.mVelocity.y = -mPlayer.mJumpStrength;
  } else {
    gravityToApply *= 0.7;
  }

  if (!mPlayer.mOnGround) {
    mPlayer.mVelocity.y = std::min(mPlayer.mTerminalVelocity, mPlayer.mVelocity.y + gravityToApply * deltaTime);
  } else {
    mPlayer.mVelocity.y = std::min(0.0f, mPlayer.mVelocity.y);
  }
}

void Player::Jumping::ChangeState() {
  std::cout << StateName() << std::endl;
  if (mPlayer.mOnLeftWall || mPlayer.mOnRightWall) {
    mPlayer.ChangeStateTo<WallSliding>();
    return;
  }

  if (!mPlayer.mJumping || mPlayer.mVelocity.y > 0) {
    mPlayer.ChangeStateTo<Falling>();
    return;
  }
}
