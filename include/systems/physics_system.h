#pragma once

#include <vector>

#include "components/collision_component.h"
#include "components/movement_component.h"
#include "components/position_component.h"

namespace platformer2d {

enum class RectangleSide { kTop, kBottom, kRight, kLeft };

struct MoverComponentAggregate {
  const CollisionComponent& collision;
  PositionComponent& position;
  MovementComponent& movement;
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
  PhysicsSystem(std::vector<MoverComponentAggregate>& mover_components,
                std::vector<ColliderComponentAggregate>& collider_components);
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
