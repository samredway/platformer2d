#include <string>

#include "components/physics_component.h"
#include "components/render_component.h"
#include "constants.h"
#include "game.h"
#include "input_handler.h"
#include "raylib.h"

namespace platformer2d {

// Game Entities
const std::string playerTag = "player";

Game::Game(int width, int height)
    : screen_width_(width), screen_height_(height), input_handler_() {
  // Setup Window
  InitWindow(screen_width_, screen_height_, "2D Platform Game");
  SetTargetFPS(kTargetFPS);

  // Initialise entities //////////////////////////////////////////////////////

  // initialise player components
  physics_components_.emplace(
      playerTag, PhysicsComponent((float)width / 2, (float)height / 2, 40, 40));
  render_components_.emplace(playerTag, RenderComponent(RED));
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
  auto player_physics = physics_components_.at(playerTag);

  // Will probably want to hand move_force onto physics component at
  // some point as different enemies will have different values and
  // anything that moves will have its own force. Not sure how that
  // will look yet so just defining here as const for now
  const float player_move_force = 10.0f;
  if (input_handler_.isRight()) {
    player_physics.applyForceX(player_move_force);
  }
  if (input_handler_.isLeft()) {
    player_physics.applyForceX(player_move_force);
  }
}

// Processors for component collections ///////////////////////////////////////
void Game::processRendering() const {
  for (const auto& render_pair : render_components_) {
    auto entity_tag = render_pair.first;
    auto physics = physics_components_.at(entity_tag);
    render_pair.second.drawRect(physics.getPositionX(), physics.getPositionY(),
                                physics.getWidth(), physics.getHeight());
  }
}

void Game::processPhysics() {
}

}  // namespace platformer2d
