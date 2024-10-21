#pragma once

#include <unordered_map>
#include <vector>

#include "components/collision_component.h"
#include "components/movement_component.h"
#include "components/position_component.h"

namespace platformer2d {

enum class RectangleSide { kTop, kBottom, kRight, kLeft };

class PhysicsSystem {
 public:
  PhysicsSystem(
      std::unordered_map<std::string, MovementComponent>& movement_components,
      std::unordered_map<std::string, PositionComponent>& position_components,
      std::unordered_map<std::string, CollisionComponent>&
          collision_components);
  void update();

 private:
  // All components for physics update
  struct PhysicsComponent {
    const CollisionComponent& collision;
    PositionComponent& position;
    MovementComponent& movement;
  };

  struct CollisionPair {
    PhysicsComponent mover;
    PhysicsComponent collider;
  };

  std::unordered_map<std::string, MovementComponent>& movement_components_;
  std::unordered_map<std::string, PositionComponent>& position_components_;
  std::unordered_map<std::string, CollisionComponent>& collision_components_;

  PhysicsComponent getPhysicsComponent(const std::string& entity_id);
  std::vector<CollisionPair> calculateCollisions(PhysicsComponent& mover);
  void resolveCollisions(std::vector<CollisionPair>& collisions);
  void updateVelocity(PhysicsComponent& mover);
  void updatePosition(PhysicsComponent& mover);
};

}  // namespace platformer2d
