#include "debugframe.h"

std::ostream& operator<<(std::ostream& os, const DebugFrame& df) {
  os << df.mInputs << " " << df.mDeltaTime;
  return os;
}

std::istream& operator>>(std::istream& is, DebugFrame& df) {
  is >> df.mInputs >> df.mDeltaTime;
  return is;
}