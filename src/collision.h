#pragma once

#include <cmath>

#include "float2.h"

enum class Collision {
  NONE,
  GROUND,
  CEILING,
  LEFTWALL,
  RIGHTWALL
};

Collision Collides(Float2 minOne, Float2 maxOne, Float2 minTwo, Float2 maxTwo);