#include "game.h"
#include "raylib.h"

using namespace platformer2d;

int main() {
  Game game{};

  while (!WindowShouldClose()) {
    game.update();
    game.draw();
  }

  return 0;
}
