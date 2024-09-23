#include <algorithm>
#include <cmath>
#include <string>
#include <unordered_map>
#include <vector>

#include "components.h"
#include "constants.h"
#include "physics_system.h"
#include "raylib.h"

namespace platformer2d {

// Forward declarations of free helper functions
enum class RectangleSide;
RectangleSide getClosestRectangleSide(const PositionComponent& position,
                                      const PositionComponent& position2);
void handleCollision(MovementComponent& movement_component,
                     PositionComponent& position,
                     const PositionComponent& position2);
void updateVelocityY(MovementComponent& movement, float delta_time);
void updateVelocityX(MovementComponent& movement, float delta_time);

PhysicsSystem::PhysicsSystem(
    std::unordered_map<std::string, MovementComponent>& movement_components,
    std::unordered_map<std::string, PositionComponent>& position_components,
    std::unordered_map<std::string, CollisionComponent>& collision_components)
    : movement_components_(movement_components),
      position_components_(position_components),
      collision_components_(collision_components) {
}

void PhysicsSystem::update() {
  const float dt = GetFrameTime();

  for (auto& movement_pair : movement_components_) {
    const std::string& mover_entity_tag = movement_pair.first;

    MovementComponent& movement_component = movement_pair.second;
    PositionComponent& position = position_components_.at(mover_entity_tag);

    std::vector<std::string> have_checked{mover_entity_tag};

    movement_component.is_grounded = false;

    for (const auto& collider_pair : collision_components_) {
      if (std::find(have_checked.begin(), have_checked.end(),
                    collider_pair.first) != have_checked.end()) {
        continue;
      }

      const PositionComponent& position2 =
          position_components_.at(collider_pair.first);

      const bool is_colliding = position.x <= position2.x + position2.width &&
                                position.x + position.width >= position2.x &&
                                position.y + position.height >= position2.y &&
                                position.y <= position2.y + position2.height;

      if (is_colliding) {
        handleCollision(movement_component, position, position2);
      }
    }

    // Update position after handling collisions
    updateVelocityY(movement_component, dt);
    position.y += movement_component.velocity_y;

    updateVelocityX(movement_component, dt);
    position.x += movement_component.velocity_x;
  }
}

// Constants for collision handling
constexpr float kCollisionOffset = 0.5f;

// Helper function implementations
enum class RectangleSide { kTop, kBottom, kRight, kLeft };

RectangleSide getClosestRectangleSide(const PositionComponent& position,
                                      const PositionComponent& position2) {
  const float bottom_diff =
      std::abs(position2.y + position2.height) - position.y;
  const float top_diff = std::abs(position.y + position.height) - position2.y;
  const float left_diff = std::abs(position.x + position.width) - position2.x;
  const float right_diff = std::abs(position2.x + position2.width) - position.x;

  const float min_dif =
      std::min({bottom_diff, top_diff, left_diff, right_diff});

  if (min_dif == bottom_diff)
    return RectangleSide::kBottom;
  else if (min_dif == top_diff)
    return RectangleSide::kTop;
  else if (min_dif == right_diff)
    return RectangleSide::kRight;
  else
    return RectangleSide::kLeft;
}

void handleCollision(MovementComponent& movement_component,
                     PositionComponent& position,
                     const PositionComponent& position2) {
  RectangleSide closest = getClosestRectangleSide(position, position2);
  switch (closest) {
    case RectangleSide::kTop:
      position.y = position2.y - position.height;
      movement_component.velocity_y = 0;
      movement_component.is_grounded = true;
      break;
    case RectangleSide::kLeft:
      position.x = position2.x - position.width - kCollisionOffset;
      movement_component.velocity_x = 0;
      movement_component.acceleration_x = 0;
      break;
    case RectangleSide::kRight:
      position.x = position2.x + position.width + kCollisionOffset;
      movement_component.velocity_x = 0;
      movement_component.acceleration_x = 0;
      break;
    case RectangleSide::kBottom:
      // TODO: Implement behavior for bottom collision
      break;
  }
}

void updateVelocityY(MovementComponent& movement, const float delta_time) {
  movement.velocity_y -= movement.acceleration_y * delta_time;

  if (!movement.is_grounded) {
    movement.velocity_y += kGravity * delta_time;
    movement.velocity_y -= movement.velocity_y * movement.drag;
  }
}

void updateVelocityX(MovementComponent& movement, const float delta_time) {
  movement.velocity_x += movement.acceleration_x * delta_time;
  movement.velocity_x -= movement.velocity_x * movement.drag;

  if (movement.is_grounded) {
    if (movement.velocity_x > 0) {
      movement.velocity_x -= movement.friction_coefficient * delta_time;
      if (movement.velocity_x < 0) movement.velocity_x = 0;
    } else if (movement.velocity_x < 0) {
      movement.velocity_x += movement.friction_coefficient * delta_time;
      if (movement.velocity_x > 0) movement.velocity_x = 0;
    }
  }
}

}  // namespace platformer2d
