#include "player.h"

void Player::SetMovingLeft(bool movingLeft) {
  mMovingLeft = movingLeft;
}

void Player::SetMovingRight(bool movingRight) {
  mMovingRight = movingRight;
}

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
    mVelocity.y = mJumpStrength;
    mOnGround = false;
  }
}

void Player::SetOnGround(bool onGround) { 
  mOnGround = onGround; 
}

void Player::UpdateHorizontalVelocity(float deltaTime) {
  if (mMovingLeft && mMovingRight) {
    return;
  }

  float deltaSpeed = mHorizontalAcceleration * deltaTime;

  if (mMovingLeft) {
    // cancel velocity if moving in opposite direction
    if (mVelocity.x > 0) {
      mVelocity.x = 0;
    }

    mVelocity.x = std::max(-mMaxHorizontalSpeed, mVelocity.x - deltaSpeed);
  } else if (mMovingRight) {
    // cancel velocity if moving in opposite direction
    if (mVelocity.x < 0) {
      mVelocity.x = 0;
    }

    mVelocity.x = std::min(mMaxHorizontalSpeed, mVelocity.x + deltaSpeed);
  } else {
    ApplyFriction(deltaTime);
  }
}

void Player::ApplyFriction(float deltaTime) {
  float friction = mOnGround ? mGroundFriction : mAirFriction;
  if (std::abs(mVelocity.x) <= friction) {
      mVelocity.x = 0.0f;
  } else {
      mVelocity.x -= (mVelocity.x > 0 ? friction : -friction);
  }
}

void Player::UpdateVerticalVelocity(float deltaTime) {
  if (!mOnGround) {
    mVelocity.y = std::max(mTerminalVelocity, mVelocity.y + mGravity * deltaTime);
  }
}

void Player::UpdatePosition(float deltaTime) {
  mPosition += mVelocity * deltaTime;
}

void Player::Update(float deltaTime) {
  UpdateHorizontalVelocity(deltaTime);
  UpdateVerticalVelocity(deltaTime);
  UpdatePosition(deltaTime);
}