#include "player.h"

void Player::Falling::UpdateHorizontalVelocity(float deltaTime) {
  const Inputs& inputs = mPlayer.mInputs.GetInputs();
  if (inputs.mLeftKeyPressed && inputs.mRightKeyPressed) {
    return;
  }

  float deltaSpeed = mPlayer.mHorizontalAcceleration * deltaTime;

  if (inputs.mLeftKeyPressed) {
    mPlayer.mVelocity.x = std::max(-mPlayer.mMaxHorizontalSpeed, mPlayer.mVelocity.x - deltaSpeed);
  } else if (inputs.mRightKeyPressed) {
    mPlayer.mVelocity.x = std::min(mPlayer.mMaxHorizontalSpeed, mPlayer.mVelocity.x + deltaSpeed);
  } else {
    mPlayer.ApplyFriction(deltaTime);
  }
}

void Player::Falling::UpdateVerticalVelocity(float deltaTime) {
  mPlayer.mVelocity.y = std::min(mPlayer.mTerminalVelocity, 
                                 mPlayer.mVelocity.y + mPlayer.mGravity * deltaTime);
}

void Player::Falling::ChangeState() {
  const Inputs& inputs = mPlayer.mInputs.GetInputs();
  if (mPlayer.mOnGround) {
    mPlayer.ChangeStateTo<Standing>();
  } else if (mPlayer.mOnLeftWall && inputs.mLeftKeyPressed) {
    mPlayer.ChangeStateTo<WallSliding>();
  } else if (mPlayer.mOnRightWall && inputs.mRightKeyPressed) {
    mPlayer.ChangeStateTo<WallSliding>();
  }
}

std::unique_ptr<Attack> Player::Falling::GetAttack() {
  std::unique_ptr<Attack> attack = mPlayer.mWeapon->GetAttack();

  // idk what the best way to format this tbh, not sure the ternary operator is the move
  Float2 front = mPlayer.mInputs.GetDirection() == Direction::RIGHT
    ? Float2(mPlayer.GetFront() + mPlayer.mWeapon->GetOffset(), mPlayer.GetTop())
    : Float2(mPlayer.GetFront() - mPlayer.mWeapon->GetOffset(), mPlayer.GetTop());

  attack->Transform(front, mPlayer.mInputs.GetDirection());

  return attack;
}
