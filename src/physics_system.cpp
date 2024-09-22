#include <string>
#include <vector>

#include "components.h"
#include "constants.h"
#include "macros.h"
#include "physics_system.h"
#include "raylib.h"

namespace platformer2d {

PhysicsSystem::PhysicsSystem(
    std::unordered_map<std::string, MovementComponent>& movement_components,
    std::unordered_map<std::string, PositionComponent>& position_components,
    std::unordered_map<std::string, CollisionComponent>& collision_components)
    : movement_components_(movement_components),
      position_components_(position_components),
      collision_components_(collision_components) {
}

void PhysicsSystem::update() {
  for (auto& movement_pair : movement_components_) {
    DLOG("Updating physics:");
    const std::string mover_entity_tag{movement_pair.first};

    // Movers components
    MovementComponent& movement_component{movement_pair.second};
    PositionComponent& position{position_components_.at(mover_entity_tag)};

    // Can reduce the number of checks by not checking aginst movers that
    // have already been checked against all colliders
    std::vector<std::string> have_checked{};

    // Obv the mover cannot collide with itself
    have_checked.push_back(mover_entity_tag);

    // Reset is grounded before we check for collisions
    movement_component.is_grounded = false;

    // Check collisions.
    for (auto& collider_pair : collision_components_) {
      if (std::find(have_checked.begin(), have_checked.end(),
                    collider_pair.first) != have_checked.end()) {
        continue;
      }

      // Get the position of the moving object
      const PositionComponent& position2{
          position_components_.at(collider_pair.first)};

      const bool is_colliding = position.x <= position2.x + position2.width &&
                                position.x + position.width >= position2.x &&
                                position.y + position.height >= position2.y &&
                                position.y <= position2.y + position2.height;

      if (is_colliding) {
        // Get distance from each side. The closest is the side colliding
        float bottom_diff =
            std::abs(position2.y + position2.height) - position.y;
        float top_diff = std::abs(position.y + position.height) - position2.y;
        float left_diff = std::abs(position.x + position.width) - position2.x;
        float right_diff = std::abs(position2.x + position2.width) - position.x;

        // Find the minimum difference to determine the side of the collision
        float min_diff =
            std::min({bottom_diff, top_diff, left_diff, right_diff});

        if (min_diff == top_diff) {
          // Collided with the top
          // Reset position to 'snap back' in case of overlap
          // Stop all movement in y direction and mark as grounded
          position.y = position2.y - position.height;
          movement_component.velocity_y = 0;
          movement_component.acceleration_y = 0;
          movement_component.is_grounded = true;
        } else if (min_diff == bottom_diff) {
          // Collided with the bottom
          // TODO
        } else if (min_diff == left_diff) {
          // Collided with the left
          // Snap back and set x movement to 0
          position.x = position2.x - position.width - 1;
          movement_component.velocity_x = 0;
          movement_component.acceleration_x = 0;
        } else if (min_diff == right_diff) {
          // Collided with the right
          // Snap back and set x movement to 0
          position.x = position2.x + position.width + 1;
          movement_component.velocity_x = 0;
          movement_component.acceleration_x = 0;
        }
      }
    }

    // Update position
    float dt = GetFrameTime();
    DLOG("  Delta time: " << dt);
    updateVelocityY(movement_component, dt);
    DLOG("  Velocity Y: " << movement_component.velocity_y);
    DLOG("  Acceleration Y: " << movement_component.acceleration_y);
    position.y += movement_component.velocity_y;
    updateVelocityX(movement_component, dt);
    position.x += movement_component.velocity_x;
    DLOG("  Position Y: " << position.y);
  }
}

void PhysicsSystem::updateVelocityY(MovementComponent& movement,
                                    float delta_time) {
  // Jump velocity
  movement.velocity_y -= movement.acceleration_y * delta_time;

  // Apply gravity
  if (!movement.is_grounded) {
    // Apply gravity to vertical velocity (v = v + gt)
    movement.velocity_y += kGravity * delta_time;
    // Apply air drag to vertical velocity (F_drag = -c * v)
    movement.velocity_y -= movement.velocity_y * movement.drag;
  }
}

void PhysicsSystem::updateVelocityX(MovementComponent& movement,
                                    float delta_time) {
  // Update velocity based on acceleration where v = v + at
  movement.velocity_x += movement.acceleration_x * delta_time;

  // Apply drag: drag reduces the velocity based on current speed
  movement.velocity_x -= movement.velocity_x * movement.drag;

  // If moving and grounded, apply friction based on the current surface
  if (movement.velocity_x > 0 && movement.is_grounded) {
    movement.velocity_x -= movement.friction_coefficient * delta_time;
    if (movement.velocity_x < 0) movement.velocity_x = 0;
  } else if (movement.velocity_x < 0 && movement.is_grounded) {
    movement.velocity_x += movement.friction_coefficient * delta_time;
    if (movement.velocity_x > 0) movement.velocity_x = 0;
  }
}

}  // namespace platformer2d
