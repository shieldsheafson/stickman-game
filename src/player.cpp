#include "player.h"

void Player::ApplyFriction(float deltaTime) {
  float friction = (mOnGround ? mGroundFriction : mAirFriction) * deltaTime;
  if (std::abs(mVelocity.x) <= friction) {
      mVelocity.x = 0.0f;
  } else {
      mVelocity.x -= (mVelocity.x > 0 ? friction : -friction);
  }
}

std::unique_ptr<Attack> Player::Update(float deltaTime, const bool* keystate) {
  mInputs.Update(keystate);
  mCurrentState->ChangeState();
  mCurrentState->UpdateHorizontalVelocity(deltaTime);
  mCurrentState->UpdateVerticalVelocity(deltaTime);
  mCurrentState->UpdatePosition(deltaTime);

  std::unique_ptr<Attack> attack = nullptr;

  if (mInputs.GetInputs().mAttackKeyPressed && mTimeTillCanAttack == 0.0f) {
    attack = mCurrentState->GetAttack();
  }

  if (attack) {
    mTimeTillCanAttack = attack->GetLengthOfAttack();
  } else {
    mTimeTillCanAttack = std::max(0.0f, mTimeTillCanAttack - deltaTime);
  }

  return attack;
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
