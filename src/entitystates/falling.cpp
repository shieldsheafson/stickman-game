#include "player.h"

void Entity::Falling::OnEnter() {
  if (mEntity.mPrievousState != "Jumping") {
    mCoyoteTime = mEntity.mCoyoteTime;
  } else {
    mCoyoteTime = 0.0f;
  }
}

void Entity::Falling::UpdateHorizontalVelocity(float deltaTime) {
  const Inputs& inputs = mEntity.mInputManager.GetInputs();
  if (inputs.mLeftKeyPressed && inputs.mRightKeyPressed) {
    return;
  }

  float deltaSpeed = mEntity.mHorizontalAcceleration * deltaTime;

  if (inputs.mLeftKeyPressed) {
    mEntity.mVelocity.x = std::max(-mEntity.mMaxHorizontalSpeed, mEntity.mVelocity.x - deltaSpeed);
  } else if (inputs.mRightKeyPressed) {
    mEntity.mVelocity.x = std::min(mEntity.mMaxHorizontalSpeed, mEntity.mVelocity.x + deltaSpeed);
  } else {
    mEntity.ApplyFriction(deltaTime);
  }
}

void Entity::Falling::UpdateVerticalVelocity(float deltaTime) {
  mEntity.mVelocity.y = std::min(mEntity.mTerminalVelocity, 
                                 mEntity.mVelocity.y + mEntity.mGravity * deltaTime);

  mCoyoteTime = std::max(mCoyoteTime - deltaTime, 0.0f);
}

void Entity::Falling::ChangeState() {
  const Inputs& inputs = mEntity.mInputManager.GetInputs();
  if (mCoyoteTime > 0 && inputs.mJumpKeyPressed && !mEntity.mInputManager.GetPreviousInputs().mJumpKeyPressed) {
    mEntity.ChangeStateTo<Jumping>();
    return;
  }

  if (mEntity.mOnGround) {
    mEntity.ChangeStateTo<Standing>();
    return;
  }
  
  if (mEntity.mOnLeftWall && inputs.mLeftKeyPressed) {
    mEntity.ChangeStateTo<WallSliding>();
    return;
  }
  
  if (mEntity.mOnRightWall && inputs.mRightKeyPressed) {
    mEntity.ChangeStateTo<WallSliding>();
    return;
  }
}

std::unique_ptr<Attack> Entity::Falling::GetAttack() {
  std::unique_ptr<Attack> attack = mEntity.mWeapon->GetAttack();

  // idk what the best way to format this tbh, not sure the ternary operator is the move
  Float2 front = mEntity.mInputManager.GetDirection() == Direction::RIGHT
    ? Float2(mEntity.GetFront() + mEntity.mWeapon->GetOffset(), mEntity.GetTop())
    : Float2(mEntity.GetFront() - mEntity.mWeapon->GetOffset(), mEntity.GetTop());

  attack->Transform(front, mEntity.mInputManager.GetDirection());

  return attack;
}
