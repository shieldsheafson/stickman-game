#include "collision.h"

Collision Collides(Float2 playerMin, Float2 playerMax, Float2 terrainMin, Float2 terrainMax) {
  if (playerMax.x < terrainMin.x || terrainMax.x < playerMin.x ||
      playerMax.y < terrainMin.y || terrainMax.y < playerMin.y) {
    return Collision::NONE;
  }

  float maxFloat = std::numeric_limits<float>::max();
  // Calculate overlap amounts
  float overlapLeft = terrainMax.x - playerMin.x;
  float overlapRight =  playerMax.x - terrainMin.x;
  float overlapTop = terrainMax.y - playerMin.y;
  float overlapBottom = playerMax.y - terrainMin.y;
  
  float maxOverlap = std::min({overlapLeft, overlapRight, overlapTop, overlapBottom});

  if (maxOverlap < 0.000001f) {
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