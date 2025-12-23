#include "inputs.h"

std::ostream& operator<<(std::ostream& os, const Inputs& i) {
  os << i.mJumpKeyPressed << " ";
  os << i.mLeftKeyPressed << " ";
  os << i.mRightKeyPressed << " ";
  os << i.mDownKeyPressed << " ";
  os << i.mAttackKeyPressed;

  return os;
}

std::istream& operator>>(std::istream& is, Inputs& i) {
  is >> i.mJumpKeyPressed >> i.mLeftKeyPressed >> i.mRightKeyPressed >> i.mDownKeyPressed >> i.mAttackKeyPressed;
  return is;
}
