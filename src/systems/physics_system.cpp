#include "systems/physics_system.h"

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
void updateVelocityY(MovementComponent& movement, const float delta_time);
void updateVelocityX(MovementComponent& movement, const float delta_time);
static Vector2 getOverlap(const Rectangle& r1, const Rectangle& r2);
static Vector2 getMinimumTranslationVector(const Vector2& overlap,
                                           const Vector2& direction);

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
  
  // Reset grounded state at the beginning of collision checks
  mover.movement.is_grounded = false;

  for (const auto& collider_pair : collision_components_) {
    if (collider_pair.first == mover.position.entity_tag) continue;

    const auto& physics_component_2 = getPhysicsComponent(collider_pair.first);
    const auto& collision_box_2 = physics_component_2.collision.getCollisionBox(
        physics_component_2.position);

    Vector2 overlap = getOverlap(collision_box_1, collision_box_2);
    if (overlap.x > 0 && overlap.y > 0) {
      Vector2 direction = {
          collision_box_1.x < collision_box_2.x ? -1.0f : 1.0f,
          collision_box_1.y < collision_box_2.y ? -1.0f : 1.0f};
      Vector2 mtv = getMinimumTranslationVector(overlap, direction);
      collisions.push_back({mover, physics_component_2, mtv, direction});
      
      // Check if this collision grounds the mover
      if (direction.y < 0 && std::abs(mtv.y) > std::abs(mtv.x)) {
        mover.movement.is_grounded = true;
      }
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
    // Apply the minimum translation vector
    collision.mover.position.x += collision.mtv.x;
    collision.mover.position.y += collision.mtv.y;

    // Update velocity
    if (collision.mtv.x != 0) {
      collision.mover.movement.velocity_x = 0;
    }
    if (collision.mtv.y != 0) {
      collision.mover.movement.velocity_y = 0;
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
Vector2 getOverlap(const Rectangle& r1, const Rectangle& r2) {
  float dx = (r1.x + r1.width / 2) - (r2.x + r2.width / 2);
  float px = (r1.width + r2.width) / 2 - std::abs(dx);
  if (px <= 0) return {0, 0};

  float dy = (r1.y + r1.height / 2) - (r2.y + r2.height / 2);
  float py = (r1.height + r2.height) / 2 - std::abs(dy);
  if (py <= 0) return {0, 0};

  return {px, py};
}

Vector2 getMinimumTranslationVector(const Vector2& overlap,
                                    const Vector2& direction) {
  if (overlap.x < overlap.y) {
    return {overlap.x * direction.x, 0};
  } else {
    return {0, overlap.y * direction.y};
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
