#include "player.h"

void Player::ApplyFriction(float deltaTime) {
  float friction = (mOnGround ? mGroundFriction : mAirFriction) * deltaTime;
  if (std::abs(mVelocity.x) <= friction) {
      mVelocity.x = 0.0f;
  } else {
      mVelocity.x -= (mVelocity.x > 0 ? friction : -friction);
  }
}

void Player::Update(float deltaTime) {
  mCurrentState->ChangeState();
  mCurrentState->UpdateHorizontalVelocity(deltaTime);
  mCurrentState->UpdateVerticalVelocity(deltaTime);
  mCurrentState->UpdatePosition(deltaTime);
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
