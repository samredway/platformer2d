#include <string>
#include <vector>

#include "physics_system.h"

namespace platformer2d {

PhysicsSystem::PhysicsSystem(
    std::unordered_map<std::string, MovementComponent>& movement_components,
    std::unordered_map<std::string, PositionComponent>& position_components,
    std::unordered_map<std::string, CollisionComponent>& collision_components)
    : movement_components_(movement_components),
      position_components_(position_components),
      collision_components_(collision_components) {
}

void PhysicsSystem::detectCollisions() {
  for (auto& movement_pair : movement_components_) {
    const std::string mover_entity_tag{movement_pair.first};

    // Movers components
    MovementComponent& movement_component{movement_pair.second};
    PositionComponent& position{position_components_.at(mover_entity_tag)};

    // Can reduce the number of checks by not checking aginst movers that
    // have already been checked against all colliders
    std::vector<std::string> have_checked{};

    // Obv the mover cannot collide with itself
    have_checked.push_back(mover_entity_tag);

    // Check collisions.
    for (auto& collider_pair : collision_components_) {
      if (std::find(have_checked.begin(), have_checked.end(),
                    collider_pair.first) != have_checked.end()) {
        continue;
      }

      // Get the position of the moving object
      const PositionComponent& position2{
          position_components_.at(collider_pair.first)};

      // Check for y collisions
      if (position.x <= position2.x + position2.width &&
          position.x + position.width >= position2.x &&
          position.y + position.height >= position2.y &&
          position.y <= position2.y + position2.height) {
        handleCollisionY(movement_component, position, position2);
      }

      // Check for x direction collisions
      if (position.y + position.height > position2.y &&
          position.y < position2.y + position2.height &&
          position.x + position.width > position2.x &&
          position.x < position2.x + position2.width) {
        handleCollisionX(movement_component, position, position2);
      }
    }
  }
}

}  // namespace platformer2d
