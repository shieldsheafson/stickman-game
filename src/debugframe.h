#pragma once

#include "inputs.h"
#include <iostream>

struct DebugFrame {
  Inputs mInputs;
  float mDeltaTime;
};

std::ostream& operator<<(std::ostream& os, const DebugFrame& df);
std::istream& operator>>(std::istream& is, DebugFrame& df);
