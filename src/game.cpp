#include <string>

#include "components/render_component.h"
#include "constants.h"
#include "game.h"
#include "macros.h"
#include "raylib.h"

namespace platformer2d {

// Game Entities
const std::string playerTag = "player";

Game::Game(int width, int height)
    : screen_width_(width), screen_height_(height) {
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
}

void Game::draw() const {
  BeginDrawing();
  ClearBackground(SKYBLUE);

  for (const auto& render_pair : render_components_) {
    const std::string entity_tag = render_pair.first;
    auto physics_it = physics_components_.find(entity_tag);
    DCHECK(physics_it != physics_components_.end())
    const PhysicsComponent& physics = physics_it->second;
    render_pair.second.drawRect(physics.getPositionX(), physics.getPositionY(),
                                physics.getWidth(), physics.getHeight());
  }
  EndDrawing();
}

}  // namespace platformer2d
