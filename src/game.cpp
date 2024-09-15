#include <string>

#include "constants.h"
#include "game.h"
#include "raylib.h"

namespace platformer2d {

// Game Entities
constexpr const char* playerTag = "player";

Game::Game(int width, int height)
    : screen_width_(width), screen_height_(height) {
  // Setup Window
  InitWindow(screen_width_, screen_height_, "2D Platform Game");
  SetTargetFPS(kTargetFPS);

  // Initialse entities //////////////////////////////////////////////////////

  // initialise player components
  physics_components_.emplace(
      std::string(playerTag),
      PhysicsComponent((float)width / 2, (float)height / 2));

  //
}

Game::~Game() {
  CloseWindow();
}

void Game::update() {
}

void Game::draw() const {
  BeginDrawing();
  ClearBackground(SKYBLUE);

  EndDrawing();
}

}  // namespace platformer2d
