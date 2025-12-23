#pragma once

#include <SDL3/SDL.h>
#include <iostream>

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

std::ostream& operator<<(std::ostream& os, const Inputs& i);

std::istream& operator>>(std::istream& is, Inputs& i);
