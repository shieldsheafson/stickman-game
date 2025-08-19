#pragma once

#include "float2.h"

#include <utility>
#include <SDL3/SDL.h>

Float2 WorldToScreen(Float2 worldCoordinates, Float2 cameraPosition, int screenWidth, int screenHeight);