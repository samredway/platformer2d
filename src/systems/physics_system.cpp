#include "systems/physics_system.h"

#include <algorithm>
#include <cmath>
#include <limits>
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

// NULL movement component with infinite mass for immovable objects
static MovementComponent IMMOVABLE("", 0, 0, 0, 0, 0, 0,
                                   std::numeric_limits<float>::infinity(), 0, 0,
                                   true, true);

// Forward declarations of free helper functions ////////////////////////////
static RectangleSide getClosestRectangleSide(const Rectangle& collision_box_1,
                                             const Rectangle& collision_box_2);
static void updateVelocityY(MovementComponent& movement, float delta_time);
static void updateVelocityX(MovementComponent& movement, float delta_time);

// Public methods /////////////////////////////////////////////////////////////
PhysicsSystem::PhysicsSystem(
    std::unordered_map<std::string, MovementComponent>& movement_components,
    std::unordered_map<std::string, PositionComponent>& position_components,
    std::unordered_map<std::string, CollisionComponent>& collision_components)
    : movement_components_(movement_components),
      position_components_(position_components),
      collision_components_(collision_components) {}

void PhysicsSystem::update() {
  for (auto& movement_pair : movement_components_) {
    PhysicsComponent mover = getPhysicsComponent(movement_pair.first);
    std::vector<CollisionPair> collisions = calculateCollisions(mover);
    resolveCollisions(collisions);
    updateVelocity(mover);
    updatePosition(mover);
  }
}

// Private methods ////////////////////////////////////////////////////////////
std::vector<PhysicsSystem::CollisionPair> PhysicsSystem::calculateCollisions(
    PhysicsComponent& mover) {
  std::vector<CollisionPair> collisions;
  const auto& collision_box_1 = mover.collision.getCollisionBox(mover.position);

  // Check each mover against each collider
  for (const auto& collider_pair : collision_components_) {
    // Skip self-collision
    if (collider_pair.first == mover.position.entity_tag) continue;

    // Get components for the collider
    const auto& physics_component_2 = getPhysicsComponent(collider_pair.first);
    const auto& collision_box_2 = physics_component_2.collision.getCollisionBox(
        physics_component_2.position);

    const bool is_colliding =
        collision_box_1.x <= collision_box_2.x + collision_box_2.width &&
        collision_box_1.x + collision_box_1.width >= collision_box_2.x &&
        collision_box_1.y + collision_box_1.height >= collision_box_2.y &&
        collision_box_1.y <= collision_box_2.y + collision_box_2.height;

    if (is_colliding) {
      collisions.push_back({mover, physics_component_2});
    } else {
      mover.movement.is_grounded = false;
    }
  }
  return collisions;
}

PhysicsSystem::PhysicsComponent PhysicsSystem::getPhysicsComponent(
    const std::string& entity_id) {
  const auto& collision =
      getComponentOrPanic<CollisionComponent>(collision_components_, entity_id);
  auto& position =
      getComponentOrPanic<PositionComponent>(position_components_, entity_id);

  // Try to get the movement component, use IMMOVABLE if not found
  auto movement_opt =
      tryGetComponent<MovementComponent>(movement_components_, entity_id);
  MovementComponent& movement =
      movement_opt.has_value() ? movement_opt->get() : IMMOVABLE;

  return PhysicsComponent{collision, position, movement};
}

void PhysicsSystem::resolveCollisions(
    std::vector<PhysicsSystem::CollisionPair>& collisions) {
  for (auto& collision : collisions) {
    const RectangleSide closest_side = getClosestRectangleSide(
        collision.mover.collision.getCollisionBox(collision.mover.position),
        collision.collider.collision.getCollisionBox(
            collision.collider.position));

    // apply an equal and opposite reaction (however we also articially set
    // velocity to 0 to prevent glitches with overshoot)
    // common sense guards before applying to stop collision still occuring
    if (closest_side == RectangleSide::kLeft &&
        // the player changes direction after initial collision
        collision.mover.movement.velocity_x > 0) {
      // todo these are wrong. we should not be setting the velocity to 0.
      // we should be applying an appropriately sized opposite force to the
      // mover which will result in a change in acceleration
      // todo we need to calculate the force based on the mass of the objects
      collision.mover.movement.velocity_x = 0;
      collision.mover.movement.acceleration_x =
          -std::abs(collision.mover.movement.acceleration_x);
      collision.collider.movement.acceleration_x =
          collision.mover.movement.acceleration_x;
    }
    if (closest_side == RectangleSide::kRight &&
        collision.mover.movement.velocity_x < 0) {
      collision.mover.movement.velocity_x = 0;
      collision.mover.movement.acceleration_x =
          std::abs(collision.mover.movement.acceleration_x);
      collision.collider.movement.acceleration_x =
          collision.mover.movement.acceleration_x;
    }
    if ((closest_side == RectangleSide::kTop &&
         collision.mover.movement.velocity_y >= 0)) {
      collision.mover.movement.velocity_y = 0;
      collision.mover.movement.is_grounded = true;
    }
    if ((closest_side == RectangleSide::kBottom &&
         collision.mover.movement.velocity_y < 0)) {
      collision.mover.movement.velocity_y = 0;
      collision.mover.movement.acceleration_y =
          -collision.mover.movement.acceleration_y;
    }
  }
}

void PhysicsSystem::updateVelocity(PhysicsComponent& mover) {
  const float delta_time = GetFrameTime();
  updateVelocityY(mover.movement, delta_time);
  updateVelocityX(mover.movement, delta_time);
}

void PhysicsSystem::updatePosition(PhysicsComponent& mover) {
  mover.position.x += mover.movement.velocity_x;
  mover.position.y += mover.movement.velocity_y;
}

// Helper function implementations ////////////////////////////////////////////
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
