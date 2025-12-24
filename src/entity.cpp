#include "entity.h"

float Entity::GetBaseTop() const  {
  if (mTextureHeight != mCurrentHeight) {
    return mPosition.y - mTextureHeight / 2;
  }
  return GetTop();
}

void Entity::ApplyFriction(float deltaTime) {
  float friction = (mOnGround ? mGroundFriction : mAirFriction) * deltaTime;
  if (std::abs(mVelocity.x) <= friction) {
      mVelocity.x = 0.0f;
  } else {
      mVelocity.x -= (mVelocity.x > 0 ? friction : -friction);
  }
}

std::unique_ptr<Attack> Entity::Update(const Inputs& inputs, float deltaTime) {
  mInputManager.Update(inputs);
  mCurrentState->ChangeState();
  mCurrentState->UpdateHorizontalVelocity(deltaTime);
  mCurrentState->UpdateVerticalVelocity(deltaTime);
  mCurrentState->UpdatePosition(deltaTime);

  std::unique_ptr<Attack> attack = nullptr;

  if (mInputManager.GetInputs().mAttackKeyPressed && mTimeTillCanAttack == 0.0f) {
    attack = mCurrentState->GetAttack();
  }

  if (attack) {
    mTimeTillCanAttack = attack->GetLengthOfAttack();
  } else {
    mTimeTillCanAttack = std::max(0.0f, mTimeTillCanAttack - deltaTime);
  }

  return attack;
}

bool Entity::AttackHits(const Attack* const attack) const {
  const Box* const hitbox = attack->GetCurrentFrame();
  if (hitbox == nullptr) { return false; }
  if (hitbox->Contains(GetTopLeft())) { return true; }
  if (hitbox->Contains(GetTopRight())) { return true; }
  if (hitbox->Contains(GetBottomLeft())) { return true; }
  if (hitbox->Contains(GetBottomRight())) { return true; }

  return false;
}
