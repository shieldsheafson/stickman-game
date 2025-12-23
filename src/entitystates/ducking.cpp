#include "player.h"

void Entity::Ducking::OnEnter() {
  mEntity.mPosition.y += mEntity.mTextureHeight / 2;
  mEntity.mCurrentHeight = mEntity.mTextureHeight / 2;
}

void Entity::Ducking::OnExit() {
  mEntity.mPosition.y -= mEntity.mTextureHeight / 2;
  mEntity.mCurrentHeight = mEntity.mTextureHeight;
}

void Entity::Ducking::UpdateHorizontalVelocity(float deltaTime) {
  const Inputs& inputs = mEntity.mInputManager.GetInputs();
  if (inputs.mLeftKeyPressed && inputs.mRightKeyPressed) {
    return;
  }

  float deltaSpeed = mEntity.mHorizontalAcceleration * mEntity.mDuckSpeedModifier * deltaTime;

  if (mEntity.OpposingVelocity()) {
    mEntity.mVelocity.x = 0;
  }

  if (inputs.mLeftKeyPressed) {
    mEntity.mVelocity.x = std::max(-mEntity.mMaxHorizontalSpeed * mEntity.mDuckSpeedModifier, mEntity.mVelocity.x - deltaSpeed);
  } else if (inputs.mRightKeyPressed) {
    mEntity.mVelocity.x = std::min(mEntity.mMaxHorizontalSpeed * mEntity.mDuckSpeedModifier, mEntity.mVelocity.x + deltaSpeed);
  } else {
    mEntity.ApplyFriction(deltaTime);
  }
}

void Entity::Ducking::UpdateVerticalVelocity(float deltaTime) {
  mEntity.mVelocity.y = 0.0f;
}

void Entity::Ducking::ChangeState() {
  const Inputs& inputs = mEntity.mInputManager.GetInputs();
  if (!mEntity.mOnGround) {
    mEntity.ChangeStateTo<Falling>();
    return;
  } 

  if (mEntity.mForceDuck) {
    return;
  }
  
  if (inputs.mJumpKeyPressed) {
    mEntity.ChangeStateTo<Jumping>();
    return;
  }

  if (!inputs.mDownKeyPressed) {
    mEntity.ChangeStateTo<Standing>();
  }
}

std::unique_ptr<Attack> Entity::Ducking::GetAttack() {
  std::unique_ptr<Attack> attack = mEntity.mWeapon->GetAttack();

  // idk what the best way to format this tbh, not sure the ternary operator is the move
  Float2 front = mEntity.mInputManager.GetDirection() == Direction::RIGHT
    ? Float2(mEntity.GetFront() + mEntity.mWeapon->GetOffset(), mEntity.GetTop())
    : Float2(mEntity.GetFront() - mEntity.mWeapon->GetOffset(), mEntity.GetTop());

  attack->Transform(front, mEntity.mInputManager.GetDirection());

  return attack;
}
