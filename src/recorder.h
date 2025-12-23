#pragma once

#include "debugframe.h"
#include "gamemode.h"

#include <vector>
#include <iostream>

struct Recorder {
  GameMode mMode;
  std::vector<DebugFrame> mFrames;
  int mCurrentIndex = 0;
};

std::ostream& operator<<(std::ostream& os, const Recorder& r);

std::istream& operator>>(std::istream& is, Recorder& r);
