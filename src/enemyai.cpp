#include "enemyai.h"

Inputs EnemyAI::GetInputs(const Enemy& enemy, const GameState& state) {
  Inputs inputs;
  Inputs prevInputs = enemy.GetPreviousInputs();

  // Lateral Movement
  int direction = mRNG() % 10;
  if (state.mPlayerHitBox.GetLeft() < enemy.GetLeft()) {
    inputs.mLeftKeyPressed = direction != 0;
    inputs.mRightKeyPressed = direction == 1;
  } else {
    inputs.mLeftKeyPressed = direction == 0;
    inputs.mRightKeyPressed = direction != 1;
  }

  // Jumping
  if (prevInputs.mJumpKeyPressed) {
    inputs.mJumpKeyPressed = mRNG() % 5 == 0;
  } else {
    inputs.mJumpKeyPressed = mRNG() % 20 == 0;
  }

  inputs.mDownKeyPressed = mRNG() % 10 == 0;
  inputs.mAttackKeyPressed = mRNG() % 50 == 0;

  return inputs;
}