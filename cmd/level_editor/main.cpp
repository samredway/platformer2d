#include "level_editor.h"
#include "raylib.h"

int main() {
  platformer2d::LevelEditor level_editor{};

  while (!WindowShouldClose()) {
    level_editor.update();
    level_editor.draw();
  }

  return 0;
}
