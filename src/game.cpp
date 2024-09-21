#include <string>

#include "components.h"
#include "constants.h"
#include "game.h"
#include "input_handler.h"
#include "macros.h"
#include "raylib.h"

namespace platformer2d {

// Game Entities
const std::string playerTag{"player"};

Game::Game(int width, int height)
    : screen_width_(width), screen_height_(height), input_handler_() {
  // Setup Window
  InitWindow(screen_width_, screen_height_, "2D Platform Game");
  SetTargetFPS(kTargetFPS);

  // Initialise entities //////////////////////////////////////////////////////
  position_components_.emplace(
      playerTag,
      PositionComponent{(float)width / 2, (float)height / 2, 40, 40});
  movement_components_.emplace(playerTag, MovementComponent{});
  render_components_.emplace(playerTag, RenderComponent{RED});
  collision_components_.emplace(playerTag, CollisionComponent{});

  // Floor tiles
  for (int i = 0; i < 8; i++) {
    const std::string tileTag{std::format("tile{}", i)};
    const float x{(width / 2.0f) - ((4.0f - i) * 40)};
    position_components_.emplace(
        tileTag, PositionComponent{x, (float)height - 40, 40, 40});
    render_components_.emplace(tileTag, RenderComponent{BLACK});
    collision_components_.emplace(tileTag, CollisionComponent{});
  }

  // Central tile to do x collision on
  const std::string tileTag{std::format("tile8")};
  const float x{(width / 2.0f) - 20};
  position_components_.emplace(
      tileTag, PositionComponent{x, (float)height - 80, 40, 40});
  render_components_.emplace(tileTag, RenderComponent{BLACK});
  collision_components_.emplace(tileTag, CollisionComponent{});

  /////////////////////////////////////////////////////////////////////////////
}

Game::~Game() {
  CloseWindow();
}

void Game::update() {
  handleInput();
  processPhysics();
}

void Game::draw() const {
  BeginDrawing();
  ClearBackground(SKYBLUE);
  processRendering();
  EndDrawing();
}

void Game::handleInput() {
  // Would crash if player not defined somehow
  auto& player_movement{movement_components_.at(playerTag)};

  input_handler_.getInput();

  const float rate_accleration{player_movement.walk_force /
                               player_movement.mass};

  // Accelerate in direction pressed.
  // If nothing pressed then decellerate unless at rest then stop
  if (input_handler_.isRight() && player_movement.is_grounded) {
    // Accelerate right
    player_movement.acceleration_x = rate_accleration;
  } else if (input_handler_.isLeft() && player_movement.is_grounded) {
    // Accelerate left
    player_movement.acceleration_x = -rate_accleration;
  } else {
    player_movement.acceleration_x = 0;
    // Artbitrary decelleration rate to mimic players own force in
    // slowing down
    if (player_movement.is_grounded) {
      player_movement.velocity_x *= 0.90;
    }
  }

  // Jump
  if (input_handler_.isSpace() && player_movement.is_grounded) {
    // Arbitrary jump force
    player_movement.acceleration_y = 6000;
  } else if (player_movement.acceleration_y > 0) {
    player_movement.acceleration_y = 0;
  }
}

// Processors for component collections
// ///////////////////////////////////////
void Game::processRendering() const {
  for (const auto& render_pair : render_components_) {
    const std::string entity_tag{render_pair.first};
    const PositionComponent& position{position_components_.at(entity_tag)};
    const Color color{render_pair.second.color};
    DrawRectangle(position.x, position.y, position.width, position.height,
                  color);
  }
}

void Game::processPhysics() {
  float dt{GetFrameTime()};
  for (auto& movement_pair : movement_components_) {
    MovementComponent& movement{movement_pair.second};
    const std::string mover_entity_tag{movement_pair.first};
    PositionComponent& position{position_components_.at(mover_entity_tag)};

    float colliding_downward_y = -1;
    float colliding_rightward = -1;
    float colliding_leftward = -1;

    // Check collisions.
    for (auto& collider_pair : collision_components_) {
      // Obv the mover cannot collide with itself
      if (collider_pair.first == mover_entity_tag) continue;
      const PositionComponent& position2{
          position_components_.at(collider_pair.first)};

      // Check for downward collision if player is at rest or moving down
      if (movement.velocity_y >= 0 &&
          position.x <= position2.x + position2.width &&
          position.x + position.width >= position2.x &&
          position.y + position.height >= position2.y &&
          position.y < position2.y + position2.height) {
        colliding_downward_y = position2.y;
      }

      auto is_colliding_x = [](const PositionComponent& position,
                               const PositionComponent& position2) {
        return position.y >= position2.y &&
               position.y <= position2.y + position2.height &&
               position.x + position.width >= position2.x &&
               position.x <= position2.x + position2.width;
      };

      // Check for right side direction collision
      if (movement.velocity_x >= 0 && is_colliding_x(position, position2)) {
        colliding_rightward = position2.x;
      }

      // Check for left side direction collision
      if (movement.velocity_x <= 0 && is_colliding_x(position, position2)) {
        colliding_leftward = position2.x + position2.width;
      }
    }

    if (colliding_rightward < 0 && colliding_leftward < 0) {
      processPhysicsX(dt, movement);
      position.x += movement.velocity_x * dt;
    } else {
      // implement collisions
      if (colliding_rightward > 0) {
        position.x = colliding_rightward - position.width - 1;
        movement.velocity_x = 0;
        movement.acceleration_x = 0;
      } else {
        position.x = colliding_leftward + 1;
        movement.velocity_x = 0;
        movement.acceleration_x = 0;
      }
    }

    if (colliding_downward_y >= 0) {
      // Adjust mover y location to sit on top of collision object
      position.y = colliding_downward_y - position.height;
      movement.velocity_y = 0;
      movement.is_grounded = true;
    } else {
      movement.is_grounded = false;
    }

    processPhysicsY(dt, movement);
    position.y += movement.velocity_y * dt;
  }
}

void Game::processPhysicsX(float delta_time, MovementComponent& movement) {
  // Update velocity based on acceleration where v = v + at
  movement.velocity_x += movement.acceleration_x * delta_time;

  // Apply drag: drag reduces the velocity based on current speed
  movement.velocity_x -= movement.velocity_x * movement.drag * delta_time;

  // If moving and grounded, apply friction based on the current surface
  if (movement.velocity_x > 0 && movement.is_grounded) {
    movement.velocity_x -= movement.friction_coefficient * delta_time;
    if (movement.velocity_x < 0) movement.velocity_x = 0;
  } else if (movement.velocity_x < 0 && movement.is_grounded) {
    movement.velocity_x += movement.friction_coefficient * delta_time;
    if (movement.velocity_x > 0) movement.velocity_x = 0;
  }
  DLOG("Is Grounded: " << movement.is_grounded);
  DLOG("Velocity X: " << movement.velocity_x);
  DLOG("Acceleration X: " << movement.acceleration_x);
}

void Game::processPhysicsY(float delta_time, MovementComponent& movement) {
  // Jump velocity
  movement.velocity_y -= movement.acceleration_y * delta_time;
  // Apply gravity to vertical velocity (v = v + g * t)
  if (!movement.is_grounded) {
    movement.velocity_y += kGravity * delta_time;
    // Apply air drag to vertical velocity (F_drag = -c * v_y)
    movement.velocity_y -= movement.velocity_y * movement.drag * delta_time;
  }
}

////////////////////////////////////////////////////////////////////////////////
}  // namespace platformer2d
