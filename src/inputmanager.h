#pragma once

#include "direction.h"
#include "inputs.h"

class InputManager {
private:
  Inputs mInputs;
  Inputs mPreviousInputs;
  Direction mDirection;
public:
  InputManager(): mInputs(), mPreviousInputs(), mDirection(Direction::RIGHT) {}

  void Update(Inputs inputs) {
    mPreviousInputs = mInputs;
    mInputs = inputs;
    if (mInputs.mLeftKeyPressed == mInputs.mRightKeyPressed) {
    } else if (mInputs.mLeftKeyPressed) {
      mDirection = Direction::LEFT;
    } else {
      mDirection = Direction::RIGHT;
    }
  }

  const Inputs& GetInputs() const { return mInputs; }
  const Inputs& GetPreviousInputs() const { return mPreviousInputs; }
  Direction GetDirection() const { return mDirection; }
};
