#pragma once

#include <vector>

#include "components/collision_component.h"
#include "components/movement_component.h"
#include "components/position_component.h"

namespace platformer2d {

enum class RectangleSide { kTop, kBottom, kRight, kLeft };

struct MoverComponentAggregate {
  MovementComponent& movement;
  PositionComponent& position;
  const CollisionComponent& collision;
};

struct ColliderComponentAggregate {
  const CollisionComponent& collision;
  PositionComponent& position;
};

struct CollisionPair {
  MoverComponentAggregate& mover;
  const ColliderComponentAggregate& collider;
  Vector2 mtv;
};

class PhysicsSystem {
 public:
  PhysicsSystem() = default;
  void init(
      std::unordered_map<std::string, MovementComponent>& movement_components,
      std::unordered_map<std::string, PositionComponent>& position_components,
      std::unordered_map<std::string, CollisionComponent>&
          collision_components);
  void update();

 private:
  std::vector<MoverComponentAggregate> mover_components_;
  std::vector<ColliderComponentAggregate> collider_components_;

  std::vector<CollisionPair> calculateCollisions(
      MoverComponentAggregate& mover);
  void resolveCollisions(std::vector<CollisionPair>& collisions);
  void updateVelocity(MoverComponentAggregate& mover);
  void updatePosition(MoverComponentAggregate& mover);
};

}  // namespace platformer2d
