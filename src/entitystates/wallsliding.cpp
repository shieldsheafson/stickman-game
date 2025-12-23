#include "player.h"

void Entity::WallSliding::UpdateHorizontalVelocity(float deltaTime) {
  mEntity.mVelocity.x = 0.0f;
}

void Entity::WallSliding::UpdateVerticalVelocity(float deltaTime) {
  float deltaSpeed = mEntity.mHorizontalAcceleration * deltaTime;

  mEntity.mVelocity.y = std::min(mEntity.mTerminalWallSlideVelocity, 
                                 mEntity.mVelocity.y + mEntity.mGravity * deltaTime);
}

void Entity::WallSliding::ChangeState() {
  const Inputs& inputs = mEntity.mInputManager.GetInputs();
  if (mEntity.mOnGround) {
    mEntity.ChangeStateTo<Standing>();
    return;
  }
  
  if (inputs.mJumpKeyPressed && !mEntity.mInputManager.GetPreviousInputs().mJumpKeyPressed) {
    mEntity.ChangeStateTo<Jumping>();
    return;
  }
  
  if (!(mEntity.mOnLeftWall || mEntity.mOnRightWall)) { // not on a wall
    mEntity.ChangeStateTo<Falling>();
    return;
  }
  
  if (mEntity.mOnLeftWall && !inputs.mLeftKeyPressed) { // on a wall but not moving into it
    mEntity.ChangeStateTo<Falling>();
    return;
  }
  
  if (mEntity.mOnRightWall && !inputs.mRightKeyPressed) { // on a wall but not moving into it
    mEntity.ChangeStateTo<Falling>();
    return;
  }
}
