#include "systems/physics_system.h"

#include <cmath>
#include <string>
#include <vector>

#include "components/component.h"
#include "components/movement_component.h"
#include "constants.h"
#include "raylib.h"

namespace platformer2d {

// Public methods /////////////////////////////////////////////////////////////
void PhysicsSystem::init(
    std::unordered_map<std::string, MovementComponent>& movement_components,
    std::unordered_map<std::string, PositionComponent>& position_components,
    std::unordered_map<std::string, CollisionComponent>& collision_components) {
  for (auto& [entity_id, movement] : movement_components) {
    mover_components_.emplace_back(
        movement, getComponentOrPanic(position_components, entity_id),
        getComponentOrPanic(collision_components, entity_id));
  }
  for (auto& [entity_id, collision] : collision_components) {
    collider_components_.emplace_back(
        getComponentOrPanic(collision_components, entity_id),
        getComponentOrPanic(position_components, entity_id));
  }
}

void PhysicsSystem::update() {
  for (auto& mover : mover_components_) {
    std::vector<CollisionPair> collisions = calculateCollisions(mover);
    resolveCollisions(collisions);
    updateVelocity(mover);
    updatePosition(mover);
  }
}

// Private methods ////////////////////////////////////////////////////////////
std::vector<CollisionPair> PhysicsSystem::calculateCollisions(
    MoverComponentAggregate& mover) {
  std::vector<CollisionPair> collisions;
  const auto& collision_box_1 = mover.collision.getCollisionBox(mover.position);

  // Reset grounded state at the beginning of collision checks
  mover.movement.is_grounded = false;

  for (const auto& collider : collider_components_) {
    if (collider.collision.entity_tag == mover.position.entity_tag) continue;
    const auto& collision_box_2 =
        collider.collision.getCollisionBox(collider.position);

    Vector2 overlap = getOverlap(collision_box_1, collision_box_2);
    if (overlap.x > 0 && overlap.y > 0) {
      Vector2 direction = {
          collision_box_1.x < collision_box_2.x ? -1.0f : 1.0f,
          collision_box_1.y < collision_box_2.y ? -1.0f : 1.0f};

      Vector2 mtv = getMinimumTranslationVector(overlap, direction);
      collisions.push_back({mover, collider, mtv});

      if (direction.y < 0 && std::abs(mtv.y) > std::abs(mtv.x)) {
        mover.movement.is_grounded = true;
      }
    }
  }
  return collisions;
}

void PhysicsSystem::resolveCollisions(std::vector<CollisionPair>& collisions) {
  for (auto& collision : collisions) {
    collision.mover.position.x += collision.mtv.x;
    collision.mover.position.y += collision.mtv.y;

    const float damping_factor = 0.8f;

    if (collision.mtv.x != 0) {
      collision.mover.movement.velocity_x *= damping_factor;
      collision.mover.movement.acceleration_x = 0;
    }
    if (collision.mtv.y != 0) {
      collision.mover.movement.velocity_y *= damping_factor;
      collision.mover.movement.acceleration_y = 0;
    }
  }
}

void PhysicsSystem::updateVelocity(MoverComponentAggregate& mover) {
  const float delta_time = GetFrameTime();
  updateVelocityY(mover.movement, delta_time);
  updateVelocityX(mover.movement, delta_time);
}

void PhysicsSystem::updatePosition(MoverComponentAggregate& mover) {
  mover.position.x += mover.movement.velocity_x;
  mover.position.y += mover.movement.velocity_y;
}

// Static helper method implementations //////////////////////////////////////
void PhysicsSystem::updateVelocityY(MovementComponent& movement,
                                    const float delta_time) {
  if (!movement.is_grounded) {
    movement.velocity_y += kGravity * delta_time;
  }
  movement.velocity_y -= movement.acceleration_y * delta_time;
  movement.velocity_y -= movement.velocity_y * movement.drag;
}

void PhysicsSystem::updateVelocityX(MovementComponent& movement,
                                    const float delta_time) {
  movement.velocity_x += movement.acceleration_x * delta_time;
  movement.velocity_x -= movement.velocity_x * movement.drag;

  if (movement.is_grounded) {
    float friction_force = movement.friction_coefficient * delta_time;
    if (std::abs(movement.velocity_x) < friction_force) {
      movement.velocity_x = 0;
    } else {
      movement.velocity_x -= std::copysign(friction_force, movement.velocity_x);
    }
  }
}

Vector2 PhysicsSystem::getOverlap(const Rectangle& rect1,
                                  const Rectangle& rect2) {
  float centerDistX = (rect1.x + rect1.width / 2) - (rect2.x + rect2.width / 2);
  float overlapX = (rect1.width + rect2.width) / 2 - std::abs(centerDistX);
  if (overlapX <= 0) return {0, 0};

  float centerDistY =
      (rect1.y + rect1.height / 2) - (rect2.y + rect2.height / 2);
  float overlapY = (rect1.height + rect2.height) / 2 - std::abs(centerDistY);
  if (overlapY <= 0) return {0, 0};

  return {overlapX, overlapY};
}

Vector2 PhysicsSystem::getMinimumTranslationVector(const Vector2& overlap,
                                                   const Vector2& direction) {
  if (overlap.x < overlap.y) {
    return {overlap.x * direction.x, 0};
  } else {
    return {0, overlap.y * direction.y};
  }
}

}  // namespace platformer2d
