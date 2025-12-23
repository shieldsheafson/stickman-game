#include "player.h"

void Entity::Standing::UpdateHorizontalVelocity(float deltaTime) {
  const Inputs& inputs = mEntity.mInputManager.GetInputs();
  if (inputs.mLeftKeyPressed && inputs.mRightKeyPressed) {
    return;
  }

  float deltaSpeed = mEntity.mHorizontalAcceleration * deltaTime;

  if (inputs.mLeftKeyPressed) {
    if (mEntity.OpposingVelocity()) { mEntity.mVelocity.x = 0; }
    mEntity.mVelocity.x = std::max(-mEntity.mMaxHorizontalSpeed, mEntity.mVelocity.x - deltaSpeed);
  } else if (inputs.mRightKeyPressed) {
    if (mEntity.OpposingVelocity()) { mEntity.mVelocity.x = 0; }
    mEntity.mVelocity.x = std::min(mEntity.mMaxHorizontalSpeed, mEntity.mVelocity.x + deltaSpeed);
  } else {
    mEntity.ApplyFriction(deltaTime);
  }
}

void Entity::Standing::UpdateVerticalVelocity(float deltaTime) {
  mEntity.mVelocity.y = 0.0f;
}

void Entity::Standing::ChangeState() {
  const Inputs& inputs = mEntity.mInputManager.GetInputs();
  if (!mEntity.mOnGround) {
    mEntity.ChangeStateTo<Falling>();
    return;
  }
  
  if (inputs.mJumpKeyPressed) {
    mEntity.ChangeStateTo<Jumping>();
    return;
  }

  if (inputs.mDownKeyPressed) {
    if (std::abs(mEntity.mVelocity.x) >= mEntity.mMaxHorizontalSpeed * mEntity.mPercentOfMaxSpeedRequiredToSlide) {
      mEntity.ChangeStateTo<GroundSliding>();
    } else {
      mEntity.ChangeStateTo<Ducking>();
    }
    return;
  }
}

std::unique_ptr<Attack> Entity::Standing::GetAttack() {
  std::unique_ptr<Attack> attack = mEntity.mWeapon->GetAttack();

  // idk what the best way to format this tbh, not sure the ternary operator is the move
  Float2 front = mEntity.mInputManager.GetDirection() == Direction::RIGHT
    ? Float2(mEntity.GetFront() + mEntity.mWeapon->GetOffset(), mEntity.GetTop())
    : Float2(mEntity.GetFront() - mEntity.mWeapon->GetOffset(), mEntity.GetTop());

  attack->Transform(front, mEntity.mInputManager.GetDirection());

  return attack;
}