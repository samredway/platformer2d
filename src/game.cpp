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

  // Tiles
  position_components_.emplace(
      "tile1", PositionComponent{(float)width / 2, (float)height - 40, 40, 40});
  render_components_.emplace("tile1", RenderComponent{BLACK});
  collision_components_.emplace("tile1", CollisionComponent{});

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

  // Accelerate in direction pressed.
  // If nothing pressed then decellerate unless at rest then stop
  if (input_handler_.isRight()) {
    player_movement.acceleration_x =
        player_movement.walk_force / player_movement.mass;
  } else if (input_handler_.isLeft()) {
    player_movement.acceleration_x =
        -(player_movement.walk_force / player_movement.mass);
  } else {
    if (player_movement.velocity_x > 0) {
      player_movement.acceleration_x =
          -(player_movement.walk_force / player_movement.mass);
    } else if (player_movement.velocity_x < 0) {
      player_movement.acceleration_x =
          player_movement.walk_force / player_movement.mass;
    } else {
      player_movement.acceleration_x = 0;
    }
  }
}

// Processors for component collections ///////////////////////////////////////
void Game::processRendering() const {
  for (const auto& render_pair : render_components_) {
    const auto& entity_tag{render_pair.first};
    // This will throw and crash the program if the position component for this
    // entity does not exist - which is fine because it means its not
    // configured correctly and should be fixed in development
    auto& position{position_components_.at(entity_tag)};
    const auto& color{render_pair.second.color};
    DrawRectangle(position.x, position.y, position.width, position.height,
                  color);
  }
}

void Game::processPhysics() {
  float dt{GetFrameTime()};
  for (auto& movement_pair : movement_components_) {
    auto& movement{movement_pair.second};
    const std::string mover_entity_tag{movement_pair.first};
    PositionComponent& position{position_components_.at(mover_entity_tag)};

    bool colliding_downward{false};

    // Check collisions.
    for (auto& collider_pair : collision_components_) {
      // Obv the mover cannot collide with itself
      if (collider_pair.first == mover_entity_tag) continue;
      const auto& position2{position_components_.at(collider_pair.first)};

      // Check for downward collision if player is at rest or moving down
      if (movement.velocity_y >= 0 &&
          position.x <= position2.x + position2.width &&
          position.x + position.width >= position2.x &&
          position.y + position.height >= position2.y) {
        colliding_downward = true;
      }
    }

    processPhysicsX(dt, movement);
    position.x += movement.velocity_x * dt;

    if (!colliding_downward || movement.velocity_y < 0) {
      processPhysicsY(dt, movement);
      position.y += movement.velocity_y * dt;
    }
  }
}

void Game::processPhysicsX(float delta_time, MovementComponent& movement) {
  // Update velocity based on acceleration where v = v + at
  movement.velocity_x += movement.acceleration_x * delta_time;
  // Apply drag: drag reduces the velocity based on current speed
  movement.velocity_x -= movement.velocity_x * movement.drag * delta_time;
  // If no acceleration, apply friction based on the current surface
  if (movement.velocity_x > 0) {
    movement.velocity_x -= movement.friction_coefficient * delta_time;
    if (movement.velocity_x < 0) movement.velocity_x = 0;
  } else if (movement.velocity_x < 0) {
    movement.velocity_x += movement.friction_coefficient * delta_time;
    if (movement.velocity_x > 0) movement.velocity_x = 0;
  }
}

void Game::processPhysicsY(float delta_time, MovementComponent& movement) {
  // Apply gravity to vertical velocity (v = v + g * t)
  movement.velocity_y += kGravity * delta_time;
  // Apply air drag to vertical velocity (F_drag = -c * v_y)
  movement.velocity_y -= movement.velocity_y * movement.drag * delta_time;
}

////////////////////////////////////////////////////////////////////////////////
}  // namespace platformer2d
