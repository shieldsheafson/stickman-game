#include "utils.h"

Float2 WorldToScreen(Float2 worldCoordinates, Float2 cameraPosition, int screenWidth, int screenHeight) {
  float relativeX = worldCoordinates.x - cameraPosition.x;
  float relativeY = worldCoordinates.y - cameraPosition.y;

  float screenX = relativeX + screenWidth / 2.0f;
  float screenY = -relativeY + screenHeight / 2.0f;
  
  return Float2(screenX, screenY);
}