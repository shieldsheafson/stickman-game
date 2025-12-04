#pragma once

#include <SDL3/SDL.h>
#include <vector>
#include <memory>

#include "float2.h"
#include "box.h"
#include "attack.h"

class Weapon {
private:
  float mDamage;
  float mOffset;
  std::vector<Box> mHitboxes;
  float mFrameDuration;

public:
  Weapon(float damage, float offset, std::vector<Box> hitboxes, float frameDuration)
  : mDamage(damage), mOffset(offset), mHitboxes(hitboxes), mFrameDuration(frameDuration) {}
  
  float GetOffset() const { return mOffset; }

  std::unique_ptr<Attack> GetAttack() const {
    std::unique_ptr<Attack> attack = std::make_unique<Attack>();
    attack->mDamage = mDamage;
    attack->mHitboxes = mHitboxes;
    attack->mFrameDuration = mFrameDuration;
    attack->mAge = 0.0f;
    return attack;
  }
};

inline std::unique_ptr<Weapon> MakeNoWeapon() {
  std::vector<Box> hitboxes;
  hitboxes.push_back(Box(0, 0, 10, 10));
  hitboxes.push_back(Box(0, 0, 20, 10));
  hitboxes.push_back(Box(0, 0, 40, 10));
  hitboxes.push_back(Box(0, 0, 50, 10));
  return std::make_unique<Weapon>(1, 20, hitboxes, .05);
}
