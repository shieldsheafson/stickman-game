#include "terrain.h"

Collision Terrain::Collision(const Player& player) const {
  if (player.GetRight() < GetLeft() || player.GetLeft() > GetRight() ||
      player.GetTop() < GetBottom() || player.GetBottom() > GetTop()) {
    return Collision::NONE;
  }
  
  // Calculate overlap amounts
  float overlapLeft = GetLeft() - player.GetRight();
  float overlapRight =  player.GetLeft() - GetRight();
  float overlapTop = GetBottom() - player.GetTop();
  float overlapBottom = player.GetBottom() - GetTop();
  
  float maxOverlap = std::max({overlapLeft, overlapRight, overlapTop, overlapBottom});

  if (maxOverlap < 0.01f && maxOverlap > -0.01f) {
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