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
        DLOG("Collision Y!");
        handleCollisionY(movement_component, position, position2);
        break;
      } else {
        movement_component.is_grounded = false;
      }

      // Check for x direction collisions
      if (position.y + position.height > position2.y &&
          position.y < position2.y + position2.height &&
          position.x + position.width > position2.x &&
          position.x < position2.x + position2.width) {
        handleCollisionX(movement_component, position, position2);
        break;
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

void PhysicsSystem::handleCollisionY(MovementComponent& movement_component,
                                     PositionComponent& mover_position,
                                     const PositionComponent& collide) {
  // Reset the movers Y so as to prevent any overlap and have it rest properly
  // on top of the collider in the case where mover is falling downward
  if (movement_component.velocity_y >= 0) {
    mover_position.y = collide.y - mover_position.height;
    movement_component.velocity_y = 0;
    movement_component.acceleration_y = 0;
    movement_component.is_grounded = true;
  }
}

void PhysicsSystem::handleCollisionX(MovementComponent& movement_component,
                                     PositionComponent& mover_position,
                                     const PositionComponent& collide) {
  if (movement_component.velocity_x > 0) {
    // -1 to move it out of collision so that it doesn't get stuck
    mover_position.x = collide.x - mover_position.width - 1;
  } else {
    mover_position.x = collide.x + collide.width + 1;
  }
  movement_component.acceleration_x = 0;
  movement_component.velocity_x = 0;
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
