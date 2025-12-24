#pragma once

#include "inputs.h"
#include "gamestate.h"
#include "enemy.h"

#include <random>

class EnemyAI {
private:
  std::default_random_engine mRNG;
public:
  EnemyAI() {}
  Inputs GetInputs(const Enemy& enemy, const GameState& state);
};
