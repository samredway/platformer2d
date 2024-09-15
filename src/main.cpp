#include "game.h"
#include "raylib.h"

int main() {
  platformer2d::Game game{800, 450};

  while (!WindowShouldClose()) {
    game.update();
    game.draw();
  }

  return 0;
}
