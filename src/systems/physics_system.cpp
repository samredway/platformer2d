#include "systems/physics_system.h"

#include <algorithm>
#include <cmath>
#include <string>
#include <unordered_map>
#include <vector>

#include "components/collision_component.h"
#include "components/component.h"
#include "components/movement_component.h"
#include "components/position_component.h"
#include "constants.h"
#include "raylib.h"

namespace platformer2d {

// Forward declarations of free helper functions
enum class RectangleSide;
RectangleSide getClosestRectangleSide(const Rectangle& collision_box_1,
                                      const Rectangle& collision_box_2);
void handleCollision(MovementComponent& movement_component,
                     PositionComponent& position,
                     const Rectangle& collision_box_1,
                     const Rectangle& collision_box_2);
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
    auto& position = getComponentOrPanic<PositionComponent>(
        position_components_, mover_entity_tag);

    std::vector<std::string> have_checked{mover_entity_tag};

    movement_component.is_grounded = false;

    for (const auto& collider_pair : collision_components_) {
      if (std::find(have_checked.begin(), have_checked.end(),
                    collider_pair.first) != have_checked.end()) {
        continue;
      }

      // Get the collision components for the mover and the collider
      const auto& collision_1 = getComponentOrPanic<CollisionComponent>(
          collision_components_, mover_entity_tag);
      const auto& collision_2 = collider_pair.second;

      const PositionComponent& position2 =
          getComponentOrPanic<PositionComponent>(position_components_,
                                                 collider_pair.first);

      const Rectangle collision_box_1 = collision_1.getCollisionBox(position);
      const Rectangle collision_box_2 = collision_2.getCollisionBox(position2);

      const bool is_colliding =
          collision_box_1.x <= collision_box_2.x + collision_box_2.width &&
          collision_box_1.x + collision_box_1.width >= collision_box_2.x &&
          collision_box_1.y + collision_box_1.height >= collision_box_2.y &&
          collision_box_1.y <= collision_box_2.y + collision_box_2.height;

      if (is_colliding) {
        handleCollision(movement_component, position, collision_box_1,
                        collision_box_2);
      }
    }

    // Update position after handling collisions
    updateVelocityY(movement_component, dt);
    position.y += movement_component.velocity_y;

    updateVelocityX(movement_component, dt);
    position.x += movement_component.velocity_x;
  }
}

// Helper function implementations
enum class RectangleSide { kTop, kBottom, kRight, kLeft };

RectangleSide getClosestRectangleSide(const Rectangle& collision_box_1,
                                      const Rectangle& collision_box_2) {
  const float bottom_diff =
      std::abs(collision_box_2.y + collision_box_2.height) - collision_box_1.y;
  const float top_diff =
      std::abs(collision_box_1.y + collision_box_1.height) - collision_box_2.y;
  const float left_diff =
      std::abs(collision_box_1.x + collision_box_1.width) - collision_box_2.x;
  const float right_diff =
      std::abs((collision_box_2.x + collision_box_2.width) - collision_box_1.x);

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

// Constants for collision offset which is the distance the mover is
// offset from the collision object so as to not get stuck in the object
constexpr float kCollisionOffset = 0.5f;

void handleCollision(MovementComponent& movement_component,
                     PositionComponent& position,
                     const Rectangle& collision_box_1,
                     const Rectangle& collision_box_2) {
  RectangleSide closest =
      getClosestRectangleSide(collision_box_1, collision_box_2);

  // position x offset for mover
  const float position_x_offset = std::abs(collision_box_1.x - position.x);
  // TODO Need to handle cases where there is a y offset for the mover
  // TODO Need to handle casee where the collider object has its own offset

  switch (closest) {
    // Mover has collided with the RectangleSide::x of the collider
    case RectangleSide::kTop:
      position.y = collision_box_2.y - collision_box_1.height;
      movement_component.velocity_y = 0;
      movement_component.is_grounded = true;
      break;
    case RectangleSide::kLeft:
      position.x = collision_box_2.x - collision_box_1.width -
                   position_x_offset - kCollisionOffset;
      movement_component.velocity_x = 0;
      movement_component.acceleration_x = 0;
      break;
    case RectangleSide::kRight:
      position.x = collision_box_2.x + collision_box_2.width -
                   position_x_offset + kCollisionOffset;
      movement_component.velocity_x = 0;
      movement_component.acceleration_x = 0;
      break;
    case RectangleSide::kBottom:
      position.y =
          collision_box_2.y + collision_box_2.height + kCollisionOffset;
      movement_component.velocity_y = 0;
      movement_component.acceleration_y = 0;
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
