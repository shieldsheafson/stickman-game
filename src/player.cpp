#include "player.h"

Float2 Player::GetPositionForCamera() const {
  Float2 positionForCamera = GetPosition();
  if (GetBaseTop() != GetTop()) {
    positionForCamera.x -= GetHeight();
    positionForCamera.y -= GetHeight() * 2;
  } else {
    positionForCamera.x -= GetHeight()/2;
    positionForCamera.y -= GetHeight()/2;
  }

  return positionForCamera;
}
