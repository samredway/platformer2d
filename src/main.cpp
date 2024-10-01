#include "constants.h"
#include "game.h"
#include "raylib.h"

using namespace platformer2d;

int main() {
  Game game{kScreenWidth, kScreenHeight};

  while (!WindowShouldClose()) {
    game.update();
    game.draw();
  }

  return 0;
}
