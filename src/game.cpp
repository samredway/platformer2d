#include <string>

#include "components.h"
#include "constants.h"
#include "game.h"
#include "input_handler.h"
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

  // initialise player components
  physics_components_.emplace(
      playerTag, PhysicsComponent{(float)width / 2, (float)height / 2, 40, 40});
  render_components_.emplace(playerTag, RenderComponent{RED});

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
  auto& player_physics{physics_components_.at(playerTag)};

  if (input_handler_.isRight()) {
    player_physics.acceleration_x +=
        player_physics.walk_force / player_physics.mass;
  }
  if (input_handler_.isLeft()) {
    player_physics.acceleration_x -=
        player_physics.walk_force / player_physics.mass;
  }
}

// Processors for component collections ///////////////////////////////////////
void Game::processRendering() const {
  for (const auto& render_pair : render_components_) {
    const auto& entity_tag{render_pair.first};
    // This will throw and crash the program if the physics component for this
    // entity does not exist.
    auto& physics{physics_components_.at(entity_tag)};
    const auto& color{render_pair.second.color};
    DrawRectangle(physics.position_x, physics.position_y, physics.width,
                  physics.height, color);
  }
}

void Game::processPhysics() {
  // auto dt = GetFrameTime();
  // for (auto& physics_pair : physics_components_) {
  //   auto physics = physics_pair.second;
  // Update velocity based on acceleration where v = v + at
  // const float velocity_x =
  //     physics.getVelocityX() + (physics.getAccelleration() * dt)
}

////////////////////////////////////////////////////////////////////////////////
}  // namespace platformer2d
