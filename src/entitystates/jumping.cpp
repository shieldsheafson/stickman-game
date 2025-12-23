#include "player.h"

void Entity::Jumping::UpdateHorizontalVelocity(float deltaTime) {
  const Inputs& inputs = mEntity.mInputManager.GetInputs();
  if (inputs.mLeftKeyPressed && inputs.mRightKeyPressed) {
    return;
  }

  float deltaSpeed = mEntity.mHorizontalAcceleration * deltaTime;
  float deltaJumpSpeed = mEntity.mJumpStrength * 25 * deltaTime;

  
  if (mEntity.mOnLeftWall && !mEntity.mOnGround) {
    mEntity.mVelocity.x = deltaJumpSpeed;
  } else if (mEntity.mOnRightWall && !mEntity.mOnGround) {
    mEntity.mVelocity.x = -deltaJumpSpeed;
  } else if (inputs.mLeftKeyPressed) {
    mEntity.mVelocity.x = std::max(-mEntity.mMaxHorizontalSpeed, mEntity.mVelocity.x - deltaSpeed);
  } else if (inputs.mRightKeyPressed) {
    mEntity.mVelocity.x = std::min(mEntity.mMaxHorizontalSpeed, mEntity.mVelocity.x + deltaSpeed);
  } else {
    mEntity.ApplyFriction(deltaTime);
  }
}

void Entity::Jumping::UpdateVerticalVelocity(float deltaTime) {
  float gravityToApply = mEntity.mGravity * mEntity.mJumpGravityModifier;
  mEntity.mVelocity.y = std::min(mEntity.mTerminalVelocity, mEntity.mVelocity.y + gravityToApply * deltaTime);
}

void Entity::Jumping::ChangeState() {
  const Inputs& inputs = mEntity.mInputManager.GetInputs();
  if (mEntity.mOnLeftWall || mEntity.mOnRightWall) {
    mEntity.ChangeStateTo<WallSliding>();
    return;
  }

  if (!inputs.mJumpKeyPressed || mEntity.mVelocity.y > 0) {
    mEntity.ChangeStateTo<Falling>();
    return;
  }
}

std::unique_ptr<Attack> Entity::Jumping::GetAttack() {
  std::unique_ptr<Attack> attack = mEntity.mWeapon->GetAttack();

  // idk what the best way to format this tbh, not sure the ternary operator is the move
  Float2 front = mEntity.mInputManager.GetDirection() == Direction::RIGHT
    ? Float2(mEntity.GetFront() + mEntity.mWeapon->GetOffset(), mEntity.GetTop())
    : Float2(mEntity.GetFront() - mEntity.mWeapon->GetOffset(), mEntity.GetTop());

  attack->Transform(front, mEntity.mInputManager.GetDirection());

  return attack;
}
