#include <algorithm>
#include <cmath>
#include <string>
#include <unordered_map>
#include <vector>

#include "components.h"
#include "constants.h"
#include "raylib.h"
#include "systems/physics_system.h"

namespace platformer2d {

// Forward declarations of free helper functions
enum class RectangleSide;
RectangleSide getClosestRectangleSide(const PositionComponent& position,
                                      const PositionComponent& position2);
void handleCollision(MovementComponent& movement_component,
                     PositionComponent& position,
                     const Vector2& offset_position,
                     const Vector2& offset_position2,
                     const CollisionComponent& collision_1,
                     const CollisionComponent& collision_2);
void updateVelocityY(MovementComponent& movement, float delta_time);
void updateVelocityX(MovementComponent& movement, float delta_time);

// Public methods
PhysicsSystem::PhysicsSystem(
    std::unordered_map<std::string, MovementComponent>& movement_components,
    std::unordered_map<std::string, PositionComponent>& position_components,
    std::unordered_map<std::string, CollisionComponent>& collision_components)
    : movement_components_(movement_components),
      position_components_(position_components),
      collision_components_(collision_components) {}

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

      // Get the collision components for the mover and the collider
      auto& collision_1 = collision_components_.at(mover_entity_tag);
      auto& collision_2 = collider_pair.second;

      const PositionComponent& position2 =
          position_components_.at(collider_pair.first);

      const float position_x = position.x + collision_1.offset_x;
      const float position_y = position.y + collision_1.offset_y;
      const float position2_x = position2.x + collision_2.offset_x;
      const float position2_y = position2.y + collision_2.offset_y;

      const bool is_colliding =
          position_x <= position2_x + collision_2.width &&
          position_x + collision_1.width >= position2_x &&
          position_y + collision_1.height >= position2_y &&
          position_y <= position2_y + collision_2.height;

      if (is_colliding) {
        const Vector2 position_offset{position_x, position_y};
        const Vector2 position2_offset{position2_x, position2_y};
        handleCollision(movement_component, position, position_offset,
                        position2_offset, collision_1, collision_2);
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

RectangleSide getClosestRectangleSide(const Vector2& position,
                                      const Vector2& position2,
                                      const CollisionComponent& collision_1,
                                      const CollisionComponent& collision_2) {
  const float bottom_diff =
      std::abs(position2.y + collision_2.height) - position.y;
  const float top_diff =
      std::abs(position.y + collision_1.height) - position2.y;
  const float left_diff =
      std::abs(position.x + collision_1.width) - position2.x;
  const float right_diff =
      std::abs(position2.x + collision_2.width) - position.x;

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
                     const Vector2& offset_position,
                     const Vector2& offset_position2,
                     const CollisionComponent& collision_1,
                     const CollisionComponent& collision_2) {
  RectangleSide closest = getClosestRectangleSide(
      offset_position, offset_position2, collision_1, collision_2);
  switch (closest) {
    case RectangleSide::kTop:
      position.y = offset_position2.y - collision_1.height;
      movement_component.velocity_y = 0;
      movement_component.is_grounded = true;
      break;
    case RectangleSide::kLeft:
      position.x = offset_position2.x - collision_1.width - kCollisionOffset;
      movement_component.velocity_x = 0;
      movement_component.acceleration_x = 0;
      break;
    case RectangleSide::kRight:
      position.x = offset_position2.x + collision_1.width + kCollisionOffset;
      movement_component.velocity_x = 0;
      movement_component.acceleration_x = 0;
      break;
    case RectangleSide::kBottom:
      position.y = offset_position2.y - collision_1.height;
      movement_component.velocity_y = 0;
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
