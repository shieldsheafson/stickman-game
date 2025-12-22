#pragma once

#include <vector>
#include <SDL3/SDL.h>

#include "direction.h"
#include "box.h"

struct Attack {
  std::vector<Box> mHitboxes;
  float mDamage;
  float mAge = 0.0f;
  float mFrameDuration;
  
  void Update(float deltaTime) {
      mAge += deltaTime;
  }
  
  const Box* GetCurrentFrame() {
    int currentFrame = static_cast<int>(mAge / mFrameDuration);
    if (currentFrame >= mHitboxes.size()) {
      return nullptr;
    }

    return &mHitboxes.at(currentFrame);
  }
  
  float GetLengthOfAttack() const {
    return mHitboxes.size() * mFrameDuration;
  }

  void Transform(Float2 front, Direction direction) {

    for (auto& hitbox : mHitboxes) {
      if (direction == Direction::LEFT) {
        hitbox = Box(hitbox.GetRight() * -1, hitbox.GetTop(), hitbox.GetWidth(), hitbox.GetHeight());
        hitbox += front;
      } else {
        hitbox += front;
      }
    }
  }
};
