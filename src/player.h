#pragma once
#include "float2.h"
#include "inputmanager.h"
#include "attack.h"
#include "weapon.h"

#include <cmath>
#include <string>
#include <SDL3/SDL.h>
#include <memory>

class Player {
public:
  class State {
  public:
    explicit State(Player& player) : mPlayer(player) {}
    virtual ~State() = default;

    virtual void OnEnter() {}
    virtual void OnExit() {}
    virtual void UpdateHorizontalVelocity(float deltaTime) = 0;
    virtual void UpdateVerticalVelocity(float deltaTime) = 0;
    void UpdatePosition(float deltaTime) { mPlayer.mPosition += mPlayer.mVelocity * deltaTime; }
    virtual void ChangeState() = 0;
    virtual std::string StateName() = 0;
    virtual std::unique_ptr<Attack> GetAttack() = 0;

  protected:
    Player& mPlayer;
  };

  class Falling : public State {
  private:
    float mCoyoteTime;
  public:
    using State::State;
    void OnEnter() override;
    void UpdateHorizontalVelocity(float deltaTime) override;
    void UpdateVerticalVelocity(float deltaTime) override;
    void ChangeState() override;
    std::string StateName() override { return "Falling"; }
    std::unique_ptr<Attack> GetAttack() override;
  };

  class Jumping : public State {
  public:
    using State::State;
    void OnEnter() override { mPlayer.mVelocity.y = -mPlayer.mJumpStrength; mPlayer.mOnGround = false; }
    void UpdateHorizontalVelocity(float deltaTime) override;
    void UpdateVerticalVelocity(float deltaTime) override;
    void ChangeState() override;
    std::string StateName() override { return "Jumping"; }
    std::unique_ptr<Attack> GetAttack() override;
  };

  class WallSliding : public State {
  public:
    using State::State;
    void UpdateHorizontalVelocity(float deltaTime) override;
    void UpdateVerticalVelocity(float deltaTime) override;
    void ChangeState() override;
    std::string StateName() override { return "WallSliding"; }
    std::unique_ptr<Attack> GetAttack() override { return nullptr; }
  };

  class GroundSliding : public State {
  private:
    float mSlideTime = 0.0f;
  public:
    using State::State;
    void OnEnter() override;
    void OnExit() override;
    void UpdateHorizontalVelocity(float deltaTime) override;
    void UpdateVerticalVelocity(float deltaTime) override;
    void ChangeState() override;
    std::string StateName() override { return "GroundSliding"; }
    std::unique_ptr<Attack> GetAttack() override { return nullptr; }
  };

  class Ducking : public State {
  public:
    using State::State;
    void OnEnter() override;
    void OnExit() override;
    void UpdateHorizontalVelocity(float deltaTime) override;
    void UpdateVerticalVelocity(float deltaTime) override;
    void ChangeState() override;
    std::string StateName() override { return "Ducking"; }
    std::unique_ptr<Attack> GetAttack() override;
  };

  class Standing : public State {
  public:
    using State::State;
  private:
    void UpdateHorizontalVelocity(float deltaTime) override;
    void UpdateVerticalVelocity(float deltaTime) override;
    void ChangeState() override;
    std::string StateName() override { return "Standing"; }
    std::unique_ptr<Attack> GetAttack() override;
  };

private:
  SDL_Texture *mTexture;
  float mTextureWidth;
  float mTextureHeight;

  float mCurrentWidth = 0;
  float mCurrentHeight = 0;

  Float2 mPosition;
  Float2 mVelocity = Float2(0.0f, 0.0f);
  
  // Movement parameters
  float mMaxHorizontalSpeed;
  float mHorizontalAcceleration;
  float mJumpStrength;
  float mGravity;
  float mTerminalVelocity;
  float mTerminalWallSlideVelocity;
  float mGroundFriction = 4000;
  float mAirFriction = 1000;
  float mMaxSlideTime = 0.5f;
  float mDuckSpeedModifier = 0.25f;
  float mJumpGravityModifier = 0.7f;
  float mPercentOfMaxSpeedRequiredToSlide = 0.8f;
  float mCoyoteTime = .2;
  
  // Input state (from keyboard input)
  InputManager mInputs;

  // Collision state (set by collision detection)
  bool mOnGround = false;
  bool mOnLeftWall = false;
  bool mOnRightWall = false;

  std::unique_ptr<State> mCurrentState = nullptr;
  std::string mPrievousState = "";
  float mTimeTillCanAttack = 0.0f;

  std::unique_ptr<Weapon> mWeapon = nullptr;

  bool OpposingVelocity() { return (mInputs.GetInputs().mLeftKeyPressed && mVelocity.x > 0) || (mInputs.GetInputs().mRightKeyPressed && mVelocity.x < 0); }
  void ApplyFriction(float deltaTime);

public:
  Player() = delete;
  Player(SDL_Texture *texture, const Float2& position, float width = -1, float height = -1,
         float maxHorizontalSpeed = 300.0f, float horizontalAcceleration = 1000.f, 
         float jumpStrength = 550.0f, float gravity = 1200.0f, 
         float terminalVelocity = 800.0f)
    : mTexture(texture), mPosition(position), 
      mTextureWidth(width), mTextureHeight(height), 
      mMaxHorizontalSpeed(maxHorizontalSpeed), 
      mHorizontalAcceleration(horizontalAcceleration), mJumpStrength(jumpStrength),
      mGravity(gravity), mTerminalVelocity(terminalVelocity), mTerminalWallSlideVelocity(terminalVelocity/4) {
        if (width == -1 || height == -1) { SDL_GetTextureSize(mTexture, &mTextureWidth, &mTextureHeight); }
        mCurrentWidth = mTextureWidth;
        mCurrentHeight = mTextureHeight;
        mWeapon = MakeNoWeapon();
        mCurrentState = std::make_unique<Standing>(*this);
        mCurrentState->OnEnter();
      }
  
  // Get Methods-----------------------------------------------------------------------------------
  SDL_Texture* GetTexture() const { return mTexture; }

  // position
  const Float2& GetPosition() const { return mPosition; }
  Float2 GetPositionForCamera() const;
  float GetBottom() const { return mPosition.y + mCurrentHeight; }
  float GetTop() const { return mPosition.y; }
  float GetLeft() const { return mPosition.x; }
  float GetRight() const { return mPosition.x + mCurrentWidth; }
  float GetFront() const { return mInputs.GetDirection() == Direction::LEFT ? GetLeft() : GetRight(); }
  float GetWidth() const { return mCurrentWidth; }
  float GetHeight() const { return mCurrentHeight; }

  const std::unique_ptr<State>& GetState() const { return mCurrentState; }

  // movement
  const Float2& GetVelocity() const { return mVelocity; }
  bool IsOnGround() const { return mOnGround; }
  bool IsOnLeftWall() const { return mOnLeftWall; }
  bool IsOnRightWall() const { return mOnRightWall; }
  // ----------------------------------------------------------------------------------------------

  // Set Methods ----------------------------------------------------------------------------------
  // position
  void SetPosition(Float2 position) { mPosition = position; }
  void SetBottom(float bottom) { mPosition.y = bottom - mCurrentHeight; }
  void SetTop(float top) { mPosition.y = top; }
  void SetLeft(float left) { mPosition.x = left; }
  void SetRight(float right) { mPosition.x = right - mCurrentWidth; }

  // movement
  void SetOnGround(bool onGround) { mOnGround = onGround; }
  void SetOnLeftWall(bool onLeftWall) { mOnLeftWall = onLeftWall; }
  void SetOnRightWall(bool onRightWall) { mOnRightWall = onRightWall; }
  void SetVelocityX(float vx) { mVelocity.x = vx; }
  void SetVelocityY(float vy) { mVelocity.y = vy; }
  void SetVelocity(const Float2& velocity) { mVelocity = velocity; }
  // ----------------------------------------------------------------------------------------------

  // misc 
  template<typename T, typename... Args>
  void ChangeStateTo(Args&&... args) {
    static_assert(std::is_base_of_v<State, T>, "T must derive from Player::State");

    mPrievousState = mCurrentState->StateName();
    mCurrentState->OnExit();
    mCurrentState = std::make_unique<T>(*this, std::forward<Args>(args)...);
    mCurrentState->OnEnter();
  }

  std::unique_ptr<Attack> Update(float deltaTime, const bool* keystate);
  Player(const Player&) = delete;
  Player& operator=(const Player&) = delete;
  Player(Player&&) noexcept = default;
  Player& operator=(Player&&) noexcept = default;
  ~Player() = default;
};