#pragma once

#include "float2.h"
#include "box.h"

struct GameState {
  Box mPlayerHitBox;
  std::vector<Box> mPlayerAttackHitboxes;

  std::vector<Box> mEnemyHitBoxes;
  std::vector<Box> mEnemyAttackHitboxes;

  std::vector<Box> mTerrain;
};
