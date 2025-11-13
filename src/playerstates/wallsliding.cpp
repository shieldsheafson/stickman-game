#include "player.h"

void Player::WallSliding::UpdateHorizontalVelocity(float deltaTime) {
  mPlayer.mVelocity.x = 0.0f;
}

void Player::WallSliding::UpdateVerticalVelocity(float deltaTime) {
  float deltaSpeed = mPlayer.mHorizontalAcceleration * deltaTime;

  mPlayer.mVelocity.y = std::min(mPlayer.mTerminalWallSlideVelocity, 
                                 mPlayer.mVelocity.y + mPlayer.mGravity * deltaTime);
}

void Player::WallSliding::ChangeState() {
  const Inputs& inputs = mPlayer.mInputs.GetInputs();
  if (mPlayer.mOnGround) {
    mPlayer.ChangeStateTo<Standing>();
    return;
  }
  
  if (inputs.mJumpKeyPressed && !mPlayer.mInputs.GetPreviousInputs().mJumpKeyPressed) {
    mPlayer.ChangeStateTo<Jumping>();
    return;
  }
  
  if (!(mPlayer.mOnLeftWall || mPlayer.mOnRightWall)) { // not on a wall
    mPlayer.ChangeStateTo<Falling>();
    return;
  }
  
  if (mPlayer.mOnLeftWall && !inputs.mLeftKeyPressed) { // on a wall but not moving into it
    mPlayer.ChangeStateTo<Falling>();
    return;
  }
  
  if (mPlayer.mOnRightWall && !inputs.mRightKeyPressed) { // on a wall but not moving into it
    mPlayer.ChangeStateTo<Falling>();
    return;
  }
}
