#include "attack.h"

const Box* const Attack::GetCurrentFrame() const {
  int currentFrame = static_cast<int>(mAge / mFrameDuration);
  if (currentFrame >= mHitboxes.size()) {
    return nullptr;
  }

  return &mHitboxes.at(currentFrame);
}

void Attack::Transform(Float2 front, Direction direction) {

  for (auto& hitbox : mHitboxes) {
    if (direction == Direction::LEFT) {
      hitbox = Box(hitbox.GetRight() * -1, hitbox.GetTop(), hitbox.GetWidth(), hitbox.GetHeight());
      hitbox += front;
    } else {
      hitbox += front;
    }
  }
}

bool Attack::Collides(const Attack& other) {
  const Box* currentFrame = GetCurrentFrame();
  const Box* otherCurrentFrame = other.GetCurrentFrame();
  if (currentFrame == nullptr || otherCurrentFrame == nullptr) {
    return false;
  }

  return currentFrame->Collides(*otherCurrentFrame) != Collision::NONE;
}
