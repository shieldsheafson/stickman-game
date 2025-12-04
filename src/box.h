#pragma once

#include <algorithm>
#include <cmath>

#include "float2.h"

#include <SDL3/SDL.h>

class Box {
  private:
    SDL_FRect rect;

  public:
    Box(Float2 a, Float2 b): 
    rect{std::min(a.x, b.x), 
         std::min(a.y, b.y), 
         std::abs(b.x - a.x), 
         std::abs(b.y - a.y)} {}

    Box(Float2 topLeft, float width, float height): 
       rect{topLeft.x, topLeft.y, width, height} {}

    Box(float left, float top, float width, float height): 
       rect{left, top, width, height} {}

    float GetTop() const { return rect.y; }
    float GetBottom() const { return rect.y + rect.h; }
    float GetLeft() const { return rect.x; }
    float GetRight() const { return rect.x + rect.w; }
    
    Float2 GetMin() const { return Float2(rect.x, rect.y); }
    Float2 GetMax() const { return Float2(rect.x + rect.w, rect.y + rect.h); }
    
    float GetWidth() const { return rect.w; }
    float GetHeight() const { return rect.h; }

    const SDL_FRect* GetSDLRect() const { return &rect; }
    SDL_FRect GetModifiedSDLRect(const Float2& camera) const { return SDL_FRect{rect.x - camera.x, rect.y - camera.y, rect.w, rect.h}; }

    Box& operator+=(const Float2& rhs);
    Box& operator-=(const Float2& rhs);

    bool Contains(const Float2& p) const {
      return p.x >= rect.x && p.y >= rect.y && p.x < rect.x + rect.w && p.y < rect.y + rect.h;
    }
};

Box operator+(const Box& lhs, const Float2& rhs);
Box operator-(const Box& lhs, const Float2& rhs);

std::ostream& operator<<(std::ostream& os, const Box& t);
std::istream& operator>>(std::istream& is, Box& t);