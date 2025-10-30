#include "collision.h"

Collision Collides(Float2 minOne, Float2 maxOne, Float2 minTwo, Float2 maxTwo) {
  if (maxOne.x < minTwo.x || minOne.x > maxTwo.x ||
      maxOne.y < minTwo.y || minOne.y > maxTwo.y) {
    return Collision::NONE;
  }
  
  // Calculate overlap amounts
  float overlapLeft = minTwo.x - maxOne.x;
  float overlapRight =  minOne.x - maxTwo.x;
  float overlapTop = minTwo.y - maxOne.y;
  float overlapBottom = minOne.y - maxTwo.y;
  // std::cout << overlapLeft << " " << overlapRight << " " << overlapTop << " " << overlapBottom << std::endl;
  
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