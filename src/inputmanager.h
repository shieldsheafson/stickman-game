#include <SDL3/SDL.h>
#include <memory>

struct Inputs {
  bool mJumpKeyPressed;
  bool mLeftKeyPressed;
  bool mRightKeyPressed;
  bool mDownKeyPressed;

  Inputs(): mJumpKeyPressed(false), mLeftKeyPressed(false),
            mRightKeyPressed(false), mDownKeyPressed(false) {}
  Inputs(const bool *keystate)
    : mJumpKeyPressed(keystate[SDL_SCANCODE_Z]), mLeftKeyPressed(keystate[SDL_SCANCODE_LEFT]),
      mRightKeyPressed(keystate[SDL_SCANCODE_RIGHT]), mDownKeyPressed(keystate[SDL_SCANCODE_DOWN]) {}
};

class InputManager {
private:
  Inputs mInputs;
  Inputs mPreviousInputs;
public:
  InputManager(): mInputs(), mPreviousInputs() {}

  void Update(const bool *keystate) {
    mPreviousInputs = mInputs;
    mInputs = Inputs(keystate);
  }

  const Inputs& GetInputs() { return mInputs; }
  const Inputs& GetPreviousInputs() { return mPreviousInputs; }
};
