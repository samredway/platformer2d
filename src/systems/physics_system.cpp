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

// Forward declarations of free helper functions ////////////////////////////
static RectangleSide getClosestRectangleSide(const Rectangle& collision_box_1,
                                             const Rectangle& collision_box_2);
// static void handlePhysicsCollision(MovementComponent& movement_component_1,
//                                    MovementComponent& movement_component_2,
//                                    RectangleSide closest);
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
  auto& movement =
      getComponentOrPanic<MovementComponent>(movement_components_, entity_id);
  return {collision, position, movement};
}

void PhysicsSystem::resolveCollisions(
    std::vector<PhysicsSystem::CollisionPair>& collisions) {
  constexpr float kCollisionOffset = 0.5f;
  for (const auto& collision_pair : collisions) {
    // position x offset for mover
    const float position_x_offset =
        std::abs(collision_pair.mover.collision
                     .getCollisionBox(collision_pair.mover.position)
                     .x -
                 collision_pair.mover.position.x);
    // TODO Need to handle cases where there is a y offset for the mover
    // TODO Need to handle casee where the collider object has its own offset
    const RectangleSide closest = getClosestRectangleSide(
        collision_pair.mover.collision.getCollisionBox(
            collision_pair.mover.position),
        collision_pair.collider.collision.getCollisionBox(
            collision_pair.collider.position));

    switch (closest) {
        // Mover has collided with the RectangleSide::x of the collider
      case RectangleSide::kTop:
        collision_pair.mover.position.y =
            collision_pair.collider.collision
                .getCollisionBox(collision_pair.collider.position)
                .y -
            collision_pair.mover.collision
                .getCollisionBox(collision_pair.mover.position)
                .height;
        collision_pair.mover.movement.velocity_y = 0;
        collision_pair.mover.movement.is_grounded = true;
        break;
      case RectangleSide::kLeft:
        collision_pair.mover.position.x =
            collision_pair.collider.collision
                .getCollisionBox(collision_pair.collider.position)
                .x -
            position_x_offset - kCollisionOffset;
        collision_pair.mover.movement.velocity_x = 0;
        collision_pair.mover.movement.acceleration_x = 0;
        break;
      case RectangleSide::kRight:
        collision_pair.mover.position.x =
            collision_pair.collider.collision
                .getCollisionBox(collision_pair.collider.position)
                .x +
            position_x_offset + kCollisionOffset;
        collision_pair.mover.movement.velocity_x = 0;
        collision_pair.mover.movement.acceleration_x = 0;
        break;
      case RectangleSide::kBottom:
        collision_pair.mover.position.y =
            collision_pair.collider.collision
                .getCollisionBox(collision_pair.collider.position)
                .y +
            kCollisionOffset;
        collision_pair.mover.movement.velocity_y = 0;
        collision_pair.mover.movement.acceleration_y = 0;
        break;
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

// with two moving or movable objects we need to calculate the new direction
// and accelleration doing a calc to make sure that the force is realistic
// only calc for the mover as the second mover will be calculated in its own
// turn
// void handlephysicscollision(movementcomponent& movement_component_1,
//                             movementcomponent& movement_component_2,
//                             rectangleside closest) {
//   // apply an equal and opposite reaction (however we also articially set
//   // velocity to 0 to prevent glitches with overshoot)
//   // common sense guards before applying to stop collision still occuring
//   when
//   // the player changes direction after initial collision
//   if (closest == rectangleside::kleft && movement_component_1.velocity_x > 0)
//   {
//     // todo these are wrong. we should not be setting the velocity to 0.
//     // we should be applying an appropriately sized opposite force to the
//     // mover which will result in a change in acceleration
//     // todo we need to calculate the force based on the mass of the objects
//     movement_component_1.velocity_x = 0;
//     movement_component_1.acceleration_x =
//         -std::abs(movement_component_1.acceleration_x);
//     movement_component_2.acceleration_x =
//     movement_component_1.acceleration_x;
//   }
//   if (closest == rectangleside::kright && movement_component_1.velocity_x <
//   0) {
//     movement_component_1.velocity_x = 0;
//     movement_component_1.acceleration_x =
//         std::abs(movement_component_1.acceleration_x);
//     movement_component_2.acceleration_x =
//     movement_component_1.acceleration_x;
//   }
//   if ((closest == RectangleSide::kTop &&
//        movement_component_1.velocity_y >= 0)) {
//     movement_component_1.velocity_y = 0;
//     movement_component_1.is_grounded = true;
//   }
//   if ((closest == RectangleSide::kBottom &&
//        movement_component_1.velocity_y < 0)) {
//     movement_component_1.velocity_y = 0;
//     movement_component_1.acceleration_y =
//     -movement_component_1.acceleration_y;
//   }
// }

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
