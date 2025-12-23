#include "player.h"

void Entity::GroundSliding::OnEnter() {
  mEntity.mPosition.y += mEntity.mTextureHeight / 2;
  mEntity.mCurrentHeight = mEntity.mTextureHeight / 2;
}

void Entity::GroundSliding::OnExit() {
  mEntity.mPosition.y -= mEntity.mTextureHeight / 2;
  mEntity.mCurrentHeight = mEntity.mTextureHeight;
}

void Entity::GroundSliding::UpdateHorizontalVelocity(float deltaTime) {
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

  mSlideTime += deltaTime;
}

void Entity::GroundSliding::UpdateVerticalVelocity(float deltaTime) {
  mEntity.mVelocity.y = 0.0f;
}

void Entity::GroundSliding::ChangeState() {
  const Inputs& inputs = mEntity.mInputManager.GetInputs();
  if (!mEntity.mOnGround) {
    mEntity.ChangeStateTo<Falling>();
    return;
  } 
  
  if (inputs.mJumpKeyPressed && !mEntity.mForceDuck) {
    mEntity.ChangeStateTo<Jumping>();
    return;
  }

  if (mSlideTime > mEntity.mMaxSlideTime || mEntity.mVelocity.x == 0 || mEntity.OpposingVelocity()) {
    if (inputs.mDownKeyPressed || mEntity.mForceDuck) {
      mEntity.ChangeStateTo<Ducking>();
    } else {
      mEntity.ChangeStateTo<Standing>();
    }
    return;
  }
}
