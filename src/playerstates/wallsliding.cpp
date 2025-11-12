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
  std::cout << StateName() << std::endl;
  if (mPlayer.mOnGround) {
    mPlayer.ChangeStateTo<Standing>();
    return;
  }
  
  if (mPlayer.mJumping && !mWasJumping) {
    mPlayer.ChangeStateTo<Jumping>();
    return;
  }
  
  if (!(mPlayer.mOnLeftWall || mPlayer.mOnRightWall)) { // not on a wall
    mPlayer.ChangeStateTo<Falling>();
    return;
  }
  
  if (mPlayer.mOnLeftWall && !mPlayer.mMovingLeft) { // on a wall but not moving into it
    mPlayer.ChangeStateTo<Falling>();
    return;
  }
  
  if (mPlayer.mOnRightWall && !mPlayer.mMovingRight) { // on a wall but not moving into it
    mPlayer.ChangeStateTo<Falling>();
    return;
  }

  mWasJumping = mWasJumping && mPlayer.mJumping;
}
