#pragma once
#include "float2.h"
#include "entity.h"

class Player : public Entity {
public:
  using Entity::Entity;

  Float2 GetPositionForCamera() const;
};
