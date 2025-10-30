#pragma once
#include "float2.h"
#include <cmath>
#include <SDL3/SDL.h>

class Player {
private:
  SDL_Texture *mTexture;
  float mTextureWidth;
  float mTextureHeight;

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
  Player(SDL_Texture *texture, const Float2& position, 
    float maxHorizontalSpeed = 300.0f, float horizontalAcceleration = 1000.f, 
    float jumpStrength = 500.0f, float gravity = -1200.0f, 
    float terminalVelocity = -800.0f)
    : mTexture(texture), mTextureHeight(0), mTextureWidth(0),
      mPosition(position), mVelocity(0, 0),
      mMaxHorizontalSpeed(maxHorizontalSpeed), 
      mHorizontalAcceleration(horizontalAcceleration), mJumpStrength(jumpStrength),
      mGravity(gravity), mTerminalVelocity(terminalVelocity),
      mGroundFriction(30), mAirFriction(5),
      mOnGround(false), mMovingLeft(false),
      mMovingRight(false) {
        SDL_GetTextureSize(mTexture, &mTextureWidth, &mTextureHeight);
      }
  
  SDL_Texture* GetTexture() const { return mTexture; }
  const Float2& GetPosition() const { return mPosition; }
  float GetBottom() const { return mPosition.y; }
  float GetTop() const { return mPosition.y + mTextureHeight; }
  float GetLeft() const { return mPosition.x; }
  float GetRight() const { return mPosition.x + mTextureWidth; }
  float GetWidth() const { return mTextureWidth; }
  float GetHeight() const { return mTextureHeight; }
  Float2& GetPosition() { return mPosition; }
  const Float2& GetVelocity() const { return mVelocity; }
  bool IsOnGround() const { return mOnGround; }

  void SetMovingLeft(bool movingLeft) { mMovingLeft = movingLeft; }
  void SetMovingRight(bool movingRight) { mMovingRight = movingRight; }
  void MoveRight();
  void MoveLeft();
  void StopHorizontalMovement() {mMovingLeft = false; mMovingRight = false;}
  void Jump();

  void SetBottom(float bottom) { mPosition.y = bottom; }
  void SetTop(float top) { mPosition.y = top - mTextureHeight; }
  void SetLeft(float left) { mPosition.x = left; }
  void SetRight(float right) { mPosition.x = right - mTextureWidth; }
  void SetOnGround(bool onGround) { mOnGround = onGround; }
  void SetVelocityX(float vx) { mVelocity.x = vx; }
  void SetVelocityY(float vy) { mVelocity.y = vy; }
  void SetVelocity(const Float2& velocity) { mVelocity = velocity; }
  
  void Update(float deltaTime);
};