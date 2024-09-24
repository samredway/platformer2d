#include "constants.h"
#include "game.h"
#include "raylib.h"

namespace platformer2d {

Game::Game(int width, int height)
    : screen_width_(width), screen_height_(height), level_(width, height) {
  // Setup Window
  InitWindow(screen_width_, screen_height_, "2D Platform Game");
  SetTargetFPS(kTargetFPS);
}

Game::~Game() {
  CloseWindow();
}

void Game::update() {
  level_.update();
}

void Game::draw() const {
  BeginDrawing();
  ClearBackground(SKYBLUE);
  level_.draw();
  EndDrawing();
}

}  // namespace platformer2d
