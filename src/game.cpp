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
    DrawRectangle(position.position_x, position.position_y, position.width,
                  position.height, color);
  }
}

void Game::processPhysics() {
  float dt{GetFrameTime()};
  for (auto& movement_pair : movement_components_) {
    auto& movement{movement_pair.second};

    // Find the new position so can check for collisions before updating the
    // component
    PositionComponent& position{position_components_.at(movement_pair.first)};
    const Rectangle mover_rect{position.position_x, position.position_y,
                               (float)position.width, (float)position.height};

    // Check collisions.
    bool colliding{false};

    // - check whether collision occured by checking the collision rectangle
    //   for the moving object against all collidables
    for (auto& collider_pair : collision_components_) {
      // Obv the mover cannot collide with itself
      if (collider_pair.first == movement_pair.first) continue;
      const PositionComponent collider_position{
          position_components_.at(collider_pair.first)};
      const Rectangle collider_rect{
          collider_position.position_x, collider_position.position_y,
          (float)collider_position.width, (float)collider_position.height};
      if (CheckCollisionRecs(mover_rect, collider_rect)) {
        colliding = true;
      }
    }

    // - if collision occurs then need to check in what direction the
    //   the object position it collided with and calculate the intersection
    //   of the objects.
    if (colliding) {
      // TEMP do not update movement
      continue;
    }

    processPhysicsX(dt, movement);
    processPhysicsY(dt, movement);

    position.position_x += movement.velocity_x * dt;
    position.position_y += movement.velocity_y * dt;
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
