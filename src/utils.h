#pragma once

#include "float2.h"

#include <utility>
#include <SDL3/SDL.h>

Float2 WorldToScreen(Float2 worldCoordinates, Float2 cameraPosition, int screenWidth, int screenHeight);

inline float TicksToTime(Uint32 ticks) { return static_cast<float>(ticks) / 1000.0f; }
inline Uint32 TimeToTicks(float time) { return static_cast<Uint32>(time) * 1000; }