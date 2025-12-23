#include "player.h"

void Player::Jumping::UpdateHorizontalVelocity(float deltaTime) {
  const Inputs& inputs = mPlayer.mInputManager.GetInputs();
  if (inputs.mLeftKeyPressed && inputs.mRightKeyPressed) {
    return;
  }

  float deltaSpeed = mPlayer.mHorizontalAcceleration * deltaTime;
  float deltaJumpSpeed = mPlayer.mJumpStrength * 25 * deltaTime;

  
  if (mPlayer.mOnLeftWall && !mPlayer.mOnGround) {
    mPlayer.mVelocity.x = deltaJumpSpeed;
  } else if (mPlayer.mOnRightWall && !mPlayer.mOnGround) {
    mPlayer.mVelocity.x = -deltaJumpSpeed;
  } else if (inputs.mLeftKeyPressed) {
    mPlayer.mVelocity.x = std::max(-mPlayer.mMaxHorizontalSpeed, mPlayer.mVelocity.x - deltaSpeed);
  } else if (inputs.mRightKeyPressed) {
    mPlayer.mVelocity.x = std::min(mPlayer.mMaxHorizontalSpeed, mPlayer.mVelocity.x + deltaSpeed);
  } else {
    mPlayer.ApplyFriction(deltaTime);
  }
}

void Player::Jumping::UpdateVerticalVelocity(float deltaTime) {
  float gravityToApply = mPlayer.mGravity * mPlayer.mJumpGravityModifier;
  mPlayer.mVelocity.y = std::min(mPlayer.mTerminalVelocity, mPlayer.mVelocity.y + gravityToApply * deltaTime);
}

void Player::Jumping::ChangeState() {
  const Inputs& inputs = mPlayer.mInputManager.GetInputs();
  if (mPlayer.mOnLeftWall || mPlayer.mOnRightWall) {
    mPlayer.ChangeStateTo<WallSliding>();
    return;
  }

  if (!inputs.mJumpKeyPressed || mPlayer.mVelocity.y > 0) {
    mPlayer.ChangeStateTo<Falling>();
    return;
  }
}

std::unique_ptr<Attack> Player::Jumping::GetAttack() {
  std::unique_ptr<Attack> attack = mPlayer.mWeapon->GetAttack();

  // idk what the best way to format this tbh, not sure the ternary operator is the move
  Float2 front = mPlayer.mInputManager.GetDirection() == Direction::RIGHT
    ? Float2(mPlayer.GetFront() + mPlayer.mWeapon->GetOffset(), mPlayer.GetTop())
    : Float2(mPlayer.GetFront() - mPlayer.mWeapon->GetOffset(), mPlayer.GetTop());

  attack->Transform(front, mPlayer.mInputManager.GetDirection());

  return attack;
}
