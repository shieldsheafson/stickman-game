#pragma once

#include <vector>
#include <SDL3/SDL.h>

#include "collision.h"
#include "direction.h"
#include "box.h"

struct Attack {
  std::vector<Box> mHitboxes;
  int mDamage;
  float mAge = 0.0f;
  float mFrameDuration;
  
  void Update(float deltaTime) { mAge += deltaTime; }
  
  const Box* const GetCurrentFrame() const;
  
  float GetLengthOfAttack() const { return mHitboxes.size() * mFrameDuration; }

  void Transform(Float2 front, Direction direction);

  bool Collides(const Attack& attack);

  void End() { mAge = mHitboxes.size() * mFrameDuration; }
};
