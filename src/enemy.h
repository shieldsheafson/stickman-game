#pragma once

#include "entity.h"

#include <string>

class Enemy : public Entity {
public:
  using Entity::Entity;
  virtual std::string GetType() const = 0;
  virtual ~Enemy() = default;
};

class BasicEnemy : public Enemy {
public:
  using Enemy::Enemy;
  std::string GetType() const override { return "Basic"; };
};
