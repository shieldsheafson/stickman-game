#pragma once
#include <SDL3/SDL.h>
#include <memory>

#include "direction.h"

struct Inputs {
  bool mJumpKeyPressed;
  bool mLeftKeyPressed;
  bool mRightKeyPressed;
  bool mDownKeyPressed;
  bool mAttackKeyPressed;

  Inputs(): mJumpKeyPressed(false), mLeftKeyPressed(false),
            mRightKeyPressed(false), mDownKeyPressed(false) {}
  Inputs(const bool *keystate)
    : mJumpKeyPressed(keystate[SDL_SCANCODE_Z]), mLeftKeyPressed(keystate[SDL_SCANCODE_LEFT]),
      mRightKeyPressed(keystate[SDL_SCANCODE_RIGHT]), mDownKeyPressed(keystate[SDL_SCANCODE_DOWN]),
      mAttackKeyPressed(keystate[SDL_SCANCODE_X]) {}
};

class InputManager {
private:
  Inputs mInputs;
  Inputs mPreviousInputs;
  Direction mDirection;
public:
  InputManager(): mInputs(), mPreviousInputs(), mDirection(Direction::RIGHT) {}

  void Update(const bool *keystate) {
    mPreviousInputs = mInputs;
    mInputs = Inputs(keystate);
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
