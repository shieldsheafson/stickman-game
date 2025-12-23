#include "player.h"

void Player::Ducking::OnEnter() {
  mPlayer.mPosition.y += mPlayer.mTextureHeight / 2;
  mPlayer.mCurrentHeight = mPlayer.mTextureHeight / 2;
}

void Player::Ducking::OnExit() {
  mPlayer.mPosition.y -= mPlayer.mTextureHeight / 2;
  mPlayer.mCurrentHeight = mPlayer.mTextureHeight;
}

void Player::Ducking::UpdateHorizontalVelocity(float deltaTime) {
  const Inputs& inputs = mPlayer.mInputManager.GetInputs();
  if (inputs.mLeftKeyPressed && inputs.mRightKeyPressed) {
    return;
  }

  float deltaSpeed = mPlayer.mHorizontalAcceleration * mPlayer.mDuckSpeedModifier * deltaTime;

  if (mPlayer.OpposingVelocity()) {
    mPlayer.mVelocity.x = 0;
  }

  if (inputs.mLeftKeyPressed) {
    mPlayer.mVelocity.x = std::max(-mPlayer.mMaxHorizontalSpeed * mPlayer.mDuckSpeedModifier, mPlayer.mVelocity.x - deltaSpeed);
  } else if (inputs.mRightKeyPressed) {
    mPlayer.mVelocity.x = std::min(mPlayer.mMaxHorizontalSpeed * mPlayer.mDuckSpeedModifier, mPlayer.mVelocity.x + deltaSpeed);
  } else {
    mPlayer.ApplyFriction(deltaTime);
  }
}

void Player::Ducking::UpdateVerticalVelocity(float deltaTime) {
  mPlayer.mVelocity.y = 0.0f;
}

void Player::Ducking::ChangeState() {
  const Inputs& inputs = mPlayer.mInputManager.GetInputs();
  if (!mPlayer.mOnGround) {
    mPlayer.ChangeStateTo<Falling>();
    return;
  } 

  if (mPlayer.mForceDuck) {
    return;
  }
  
  if (inputs.mJumpKeyPressed) {
    mPlayer.ChangeStateTo<Jumping>();
    return;
  }

  if (!inputs.mDownKeyPressed) {
    mPlayer.ChangeStateTo<Standing>();
  }
}

std::unique_ptr<Attack> Player::Ducking::GetAttack() {
  std::unique_ptr<Attack> attack = mPlayer.mWeapon->GetAttack();

  // idk what the best way to format this tbh, not sure the ternary operator is the move
  Float2 front = mPlayer.mInputManager.GetDirection() == Direction::RIGHT
    ? Float2(mPlayer.GetFront() + mPlayer.mWeapon->GetOffset(), mPlayer.GetTop())
    : Float2(mPlayer.GetFront() - mPlayer.mWeapon->GetOffset(), mPlayer.GetTop());

  attack->Transform(front, mPlayer.mInputManager.GetDirection());

  return attack;
}
