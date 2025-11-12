#include "collision.h"

Collision Collides(Float2 playerMin, Float2 playerMax, Float2 terrainMin, Float2 terrainMax) {
  if (playerMax.x < terrainMin.x || terrainMax.x < playerMin.x ||
      playerMax.y < terrainMin.y || terrainMax.y < playerMin.y) {
    return Collision::NONE;
  }

  // Calculate overlap amounts
  float overlapLeft = terrainMax.x - playerMin.x;
  float overlapRight =  playerMax.x - terrainMin.x;
  float overlapTop = terrainMax.y - playerMin.y;
  float overlapBottom = playerMax.y - terrainMin.y;
  
  float minOverlap = std::min({overlapLeft, overlapRight, overlapTop, overlapBottom});

  // Ground collision
  if (minOverlap == overlapBottom) {
    return Collision::GROUND;
  }
  
  // Ceiling collision
  if (minOverlap == overlapTop) {
    return Collision::CEILING;
  }

  // Left wall collision
  if (minOverlap == overlapRight) {
    return Collision::RIGHTWALL;
  }

  // Right wall collision
  return Collision::LEFTWALL;
}