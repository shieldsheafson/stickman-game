#pragma once
#include "float2.h"
#include <cmath>
#include <SDL3/SDL.h>

class Player {
private:
  SDL_Texture **mTexture;

  Float2 mPosition;
  Float2 mVelocity;
  
  // Movement parameters
  float mMaxHorizontalSpeed;
  float mHorizontalAcceleration;
  float mJumpStrength;
  float mGravity;
  float mTerminalVelocity;
  float mGroundFriction;
  float mAirFriction;
  
  // State
  bool mOnGround;
  
  // Input state
  bool mMovingLeft;
  bool mMovingRight;

  void UpdateHorizontalVelocity(float deltaTime);
  void UpdateVerticalVelocity(float deltaTime);
  void UpdatePosition(float deltaTime);
  void ApplyFriction(float deltaTime);

public:
  Player(SDL_Texture **texture, const Float2& position, 
    float maxHorizontalSpeed = 300.0f, float horizontalAcceleration = 1000.f, 
    float jumpStrength = 500.0f, float gravity = -1200.0f, 
    float terminalVelocity = -800.0f)
    : mTexture(texture), mPosition(position), mVelocity(0, 0),
      mMaxHorizontalSpeed(maxHorizontalSpeed), 
      mHorizontalAcceleration(horizontalAcceleration), mJumpStrength(jumpStrength),
      mGravity(gravity), mTerminalVelocity(terminalVelocity),
      mGroundFriction(30), mAirFriction(5),
      mOnGround(false), mMovingLeft(false),
      mMovingRight(false) {}
  
  // Getters
  SDL_Texture* GetTexture() const { return *mTexture; }
  const Float2& GetPosition() const { return mPosition; }
  Float2& GetPosition() { return mPosition; }
  const Float2& GetVelocity() const { return mVelocity; }
  bool IsOnGround() const { return mOnGround; }
  
  // Input handling
  void SetMovingLeft(bool moving);
  void SetMovingRight(bool moving);
  void MoveRight();
  void MoveLeft();
  void StopHorizontalMovement() {mMovingLeft = false; mMovingRight = false;}
  void Jump();

  // State management
  void SetOnGround(bool onGround);
  
  void Update(float deltaTime);
};