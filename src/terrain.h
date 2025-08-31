#pragma once

#include "collision.h"
#include "float2.h"
#include "player.h"

#include <SDL3/SDL.h>

class Terrain {
  private:
    Float2 mMin;
    Float2 mMax;

  public:
    Terrain(Float2 a, Float2 b): mMin(a), mMax(b) {
      if (a.x > b.x) {
        std::swap(mMin.x, mMax.x);
      }
      if (a.y > b.y) {
        std::swap(mMin.y, mMax.y);
      }
    }

    Terrain(Float2 topRight, float height, float width): mMin(topRight), mMax(Float2(topRight.x + width, topRight.y + height)) {}

    float GetTop() const { return mMax.y; }
    float GetBottom() const { return mMin.y; }
    float GetLeft() const { return mMin.x; }
    float GetRight() const { return mMax.x; }
    
    Float2 GetMin() const { return mMin; }
    Float2 GetMax() const { return mMax; }
    
    float GetWidth() const { return mMax.x - mMin.x; }
    float GetHeight() const { return mMax.y - mMin.y; }

    bool Contains(const Float2& p) const {
      return p.x > mMin.x && p.y > mMin.y && p.x < mMax.x && p.y < mMax.y;
    }

    Collision Collision(const Player& player) const;
};