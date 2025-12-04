#include "player.h"

void Player::Jumping::UpdateHorizontalVelocity(float deltaTime) {
  const Inputs& inputs = mPlayer.mInputs.GetInputs();
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
  float deltaSpeed = mPlayer.mHorizontalAcceleration * deltaTime;
  float gravityToApply = mPlayer.mGravity;

  if (mPlayer.mOnGround) {
    mPlayer.mVelocity.y = -mPlayer.mJumpStrength;
    mPlayer.mOnGround = false;
  } else if (mPlayer.mOnLeftWall || mPlayer.mOnRightWall) {
    mPlayer.mVelocity.y = -mPlayer.mJumpStrength;
  } else {
    gravityToApply *= mPlayer.mJumpGravityModifier;
  }

  if (!mPlayer.mOnGround) {
    mPlayer.mVelocity.y = std::min(mPlayer.mTerminalVelocity, mPlayer.mVelocity.y + gravityToApply * deltaTime);
  } else {
    mPlayer.mVelocity.y = std::min(0.0f, mPlayer.mVelocity.y);
  }
}

void Player::Jumping::ChangeState() {
  const Inputs& inputs = mPlayer.mInputs.GetInputs();
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
  Float2 front = mPlayer.mInputs.GetDirection() == Direction::RIGHT
    ? Float2(mPlayer.GetFront() + mPlayer.mWeapon->GetOffset(), mPlayer.GetTop())
    : Float2(mPlayer.GetFront() - mPlayer.mWeapon->GetOffset(), mPlayer.GetTop());

  attack->Transform(front, mPlayer.mInputs.GetDirection());

  return attack;
}
