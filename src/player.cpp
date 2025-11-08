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

  if (mDucking || mSliding) {
    mSliding = mCurrentSlideTime < mMaxSlideTime && (mMovingLeft || mMovingRight);
  } else {
    mCurrentSlideTime = 0.0f;
  }

  float deltaSpeed = mHorizontalAcceleration * deltaTime;

  if (mMovingLeft) {
    // cancel velocity if moving in opposite direction
    if (mVelocity.x > 0 && mOnGround) {
      mVelocity.x = 0;
    }

    if (mDucking && mOnGround && !mSliding) {
      mVelocity.x = std::max(-mMaxHorizontalSpeed/8, mVelocity.x - deltaSpeed);
    } else {
      mVelocity.x = std::max(-mMaxHorizontalSpeed, mVelocity.x - deltaSpeed);
    }
  } else if (mMovingRight) {
    // cancel velocity if moving in opposite direction
    if (mVelocity.x < 0 && mOnGround) {
      mVelocity.x = 0;
    }

    if (mDucking && mOnGround && !mSliding) {
      mVelocity.x = std::min(mMaxHorizontalSpeed/8, mVelocity.x + deltaSpeed);
    } else {
      mVelocity.x = std::min(mMaxHorizontalSpeed, mVelocity.x + deltaSpeed);
    }
  } else {
    ApplyFriction(deltaTime);
  }

  mCurrentSlideTime += deltaTime;
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
  float gravityToApply = mGravity;

  if (mJumping) {
    if (mOnGround) {
      mVelocity.y = -mJumpStrength;
      mOnGround = false;
    } else if (mVelocity.y < 0) {
      gravityToApply *= 0.7;
    } if (mOnLeftWall && !mWasJumping) {
      mVelocity.y = -mJumpStrength;
      mVelocity.x = -mJumpStrength/2;
    } else if (mOnRightWall && !mWasJumping) {
      mVelocity.y = -mJumpStrength;
      mVelocity.x = mJumpStrength/2;
    }
  }

  if (mOnLeftWall || mOnRightWall) {
    mVelocity.y = std::min(mWallSlideVeloctiy, mVelocity.y + gravityToApply * deltaTime);
  } else if (!mOnGround) {
    mVelocity.y = std::min(mTerminalVelocity, mVelocity.y + gravityToApply * deltaTime);
  } else {
    mVelocity.y = std::min(0.0f, mVelocity.y);
  }
}

void Player::UpdatePosition(float deltaTime) {
  mPosition += mVelocity * deltaTime;
}

void Player::UpdateState() {
  mOnRightWall = false;
  mOnLeftWall = false;
  mWasJumping = mJumping;

  if (mDucking || mSliding) {
    if (!(mWasDucking || mWasSliding)) {
      mPosition.y += mTextureHeight / 2;
    }
    mCurrentHeight = mTextureHeight / 2;
  } else {
    if (mWasDucking || mWasSliding) {
      mPosition.y -= mTextureHeight / 2;
    }
    mCurrentHeight = mTextureHeight;
  }

  mWasDucking = mDucking;
  mWasSliding = mSliding;
}

void Player::Update(float deltaTime) {
  UpdateHorizontalVelocity(deltaTime);
  UpdateVerticalVelocity(deltaTime);
  UpdatePosition(deltaTime);
  UpdateState();
}

Float2 Player::GetPositionForCamera() const {
  Float2 positionForCamera = mPosition;
  if (mTextureHeight != mCurrentHeight) {
    positionForCamera.y -= mCurrentHeight;
  }

  positionForCamera.x -= mTextureWidth/2;
  positionForCamera.y -= mTextureHeight/2;

  return positionForCamera;
}