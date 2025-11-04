#include "collision.h"

Collision Collides(Float2 minOne, Float2 maxOne, Float2 minTwo, Float2 maxTwo) {
  if (maxOne.x < minTwo.x || minOne.x > maxTwo.x ||
      maxOne.y < minTwo.y || minOne.y > maxTwo.y) {
    return Collision::NONE;
  }

  // Calculate overlap amounts
  float overlapLeft = abs(minTwo.x - maxOne.x);
  float overlapRight =  abs(minOne.x - maxTwo.x);
  float overlapTop = abs(minTwo.y - maxOne.y);
  float overlapBottom = abs(minOne.y - maxTwo.y);
  
  float maxOverlap = std::max({overlapLeft, overlapRight, overlapTop, overlapBottom});

  if (maxOverlap < 0.000001f && maxOverlap > -0.000001f) {
    return Collision::NONE;
  }

  // Ground collision
  if (maxOverlap == overlapBottom) {
    return Collision::GROUND;
  }
  
  // Ceiling collision
  if (maxOverlap == overlapTop) {
    return Collision::CEILING;
  }

  // Left wall collision
  if (maxOverlap == overlapRight) {
    return Collision::LEFTWALL;
  }

  // Right wall collision
  return Collision::RIGHTWALL;
}