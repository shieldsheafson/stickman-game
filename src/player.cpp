#include "player.h"

void Player::MoveRight() {
  mMovingRight = true;
  mMovingLeft = false;
}

void Player::MoveLeft() {
  mMovingRight = false;
  mMovingLeft = true;
}

void Player::Jump() {
  if (mOnGround) {
    mVelocity.y = -mJumpStrength;
    mOnGround = false;
  }
}

void Player::UpdateHorizontalVelocity(float deltaTime) {
  if (mMovingLeft && mMovingRight) {
    return;
  }

  float deltaSpeed = mHorizontalAcceleration * deltaTime;

  if (mMovingLeft) {
    // cancel velocity if moving in opposite direction
    if (mVelocity.x > 0 && mOnGround) {
      mVelocity.x = 0;
    }

    mVelocity.x = std::max(-mMaxHorizontalSpeed, mVelocity.x - deltaSpeed);
  } else if (mMovingRight) {
    // cancel velocity if moving in opposite direction
    if (mVelocity.x < 0 && mOnGround) {
      mVelocity.x = 0;
    }

    mVelocity.x = std::min(mMaxHorizontalSpeed, mVelocity.x + deltaSpeed);
  } else {
    ApplyFriction(deltaTime);
  }
}

void Player::ApplyFriction(float deltaTime) {
  float friction = (mOnGround ? mGroundFriction : mAirFriction) * deltaTime;
  if (std::abs(mVelocity.x) <= friction) {
      mVelocity.x = 0.0f;
  } else {
      mVelocity.x -= (mVelocity.x > 0 ? friction : -friction);
  }
}

void Player::UpdateVerticalVelocity(float deltaTime) {
  float deltaSpeed = mHorizontalAcceleration * deltaTime;

  if (mJumping) {
    if (mOnGround) {
      mVelocity.y = -mJumpStrength;
      mOnGround = false;
    } else if (mVelocity.y < -mJumpStrength/2) { // still in the middle of jump
      mVelocity.y -= mJumpStrength * deltaTime; // add a little to the jump
    } else if (mOnLeftWall) {
      mVelocity.y = -mJumpStrength;
      mVelocity.x = -mJumpStrength/2;
    } else if (mOnRightWall) {
      mVelocity.y = -mJumpStrength;
      mVelocity.x = mJumpStrength/2;
    }
  }

  if (!mOnGround) {
    mVelocity.y = std::min(mTerminalVelocity, mVelocity.y + mGravity * deltaTime);
  } else {
    mVelocity.y = std::min(0.0f, mVelocity.y);
  }

  mOnRightWall = false;
  mOnLeftWall = false;
}

void Player::UpdatePosition(float deltaTime) {
  mPosition += mVelocity * deltaTime;
}

void Player::Update(float deltaTime) {
  UpdateHorizontalVelocity(deltaTime);
  UpdateVerticalVelocity(deltaTime);
  UpdatePosition(deltaTime);
}