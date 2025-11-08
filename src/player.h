#pragma once
#include "float2.h"
#include <cmath>
#include <SDL3/SDL.h>

class Player {
private:
  SDL_Texture *mTexture;
  float mTextureWidth;
  float mTextureHeight;

  float mCurrentWidth;
  float mCurrentHeight;

  Float2 mPosition;
  Float2 mVelocity;
  
  // Movement parameters
  float mMaxHorizontalSpeed;
  float mHorizontalAcceleration;
  float mJumpStrength;
  float mGravity;
  float mTerminalVelocity;
  float mWallSlideVeloctiy;
  float mGroundFriction;
  float mAirFriction;
  float mMaxSlideTime;
  
  // State
  bool mOnGround = false;
  bool mOnLeftWall = false;
  bool mOnRightWall = false;
  bool mCanWallJump = false;
  bool mWasJumping = false;
  bool mWasDucking = false;
  bool mWasSliding = false;
  float mCurrentSlideTime = 0.0f;
  
  // Input state
  bool mMovingLeft = false;
  bool mMovingRight = false;
  bool mJumping = false;
  bool mDucking = false;
  bool mSliding = false;

  void UpdateHorizontalVelocity(float deltaTime);
  void UpdateVerticalVelocity(float deltaTime);
  void UpdatePosition(float deltaTime);
  void UpdateState();
  void ApplyFriction(float deltaTime);

public:
  Player(): mTexture(NULL) {}
  Player(SDL_Texture *texture, const Float2& position, float width = -1, float height = -1,
    float maxHorizontalSpeed = 300.0f, float horizontalAcceleration = 1000.f, 
    float jumpStrength = 550.0f, float gravity = 1200.0f, 
    float terminalVelocity = 800.0f)
    : mTexture(texture), mTextureHeight(height), mTextureWidth(width),
      mCurrentWidth(0), mCurrentHeight(0),
      mPosition(position), mVelocity(0, 0),
      mMaxHorizontalSpeed(maxHorizontalSpeed), 
      mHorizontalAcceleration(horizontalAcceleration), mJumpStrength(jumpStrength),
      mGravity(gravity), mTerminalVelocity(terminalVelocity), mWallSlideVeloctiy(terminalVelocity/4),
      mGroundFriction(4000), mAirFriction(1000), mMaxSlideTime(0.5f) {
        if (width == -1 || height == -1) { SDL_GetTextureSize(mTexture, &mTextureWidth, &mTextureHeight); }
        mCurrentWidth = mTextureWidth;
        mCurrentHeight = mTextureHeight;
      }
  
  SDL_Texture* GetTexture() const { return mTexture; }
  const Float2& GetPosition() const { return mPosition; }
  Float2 GetPositionForCamera() const;
  float GetBottom() const { return mPosition.y + mCurrentHeight; }
  float GetTop() const { return mPosition.y; }
  float GetLeft() const { return mPosition.x; }
  float GetRight() const { return mPosition.x + mCurrentWidth; }
  float GetWidth() const { return mCurrentWidth; }
  float GetHeight() const { return mCurrentHeight; }
  Float2& GetPosition() { return mPosition; }
  const Float2& GetVelocity() const { return mVelocity; }
  bool IsOnGround() const { return mOnGround; }
  bool IsOnLeftWall() const { return mOnLeftWall; }
  bool IsOnRightWall() const { return mOnRightWall; }
  bool IsDucking() const { return mDucking; }
  bool IsSliding() const { return mSliding; }

  void SetMovingLeft(bool movingLeft) { mMovingLeft = movingLeft; }
  void SetMovingRight(bool movingRight) { mMovingRight = movingRight; }
  void SetJumping(bool jumping) { mJumping = jumping; }
  void SetDucking(bool ducking) { mDucking = ducking; }
  void MoveRight();
  void MoveLeft();
  void StopHorizontalMovement() {mMovingLeft = false; mMovingRight = false;}
  void Jump();

  void SetBottom(float bottom) { mPosition.y = bottom - mCurrentHeight; }
  void SetTop(float top) { mPosition.y = top; }
  void SetLeft(float left) { mPosition.x = left; }
  void SetRight(float right) { mPosition.x = right - mCurrentWidth; }
  void SetOnGround(bool onGround) { mOnGround = onGround; }
  void SetOnLeftWall(bool onLeftWall) { mOnLeftWall = onLeftWall; }
  void SetOnRightWall(bool onRightWall) { mOnRightWall = onRightWall; }
  void SetVelocityX(float vx) { mVelocity.x = vx; }
  void SetVelocityY(float vy) { mVelocity.y = vy; }
  void SetVelocity(const Float2& velocity) { mVelocity = velocity; }
  
  void Update(float deltaTime);
};
