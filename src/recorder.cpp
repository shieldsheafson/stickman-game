#include "recorder.h"

std::ostream& operator<<(std::ostream& os, const Recorder& r) {
  for (const auto& frame : r.mFrames) {
    os << frame << std::endl;
  }
  return os;
}

std::istream& operator>>(std::istream& is, Recorder& r) {
  r.mFrames.clear();
  DebugFrame frame;
  while (is >> frame) {
    r.mFrames.push_back(frame);
  }

  return is;
}
