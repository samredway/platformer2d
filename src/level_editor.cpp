#include "level_editor.h"

#include "constants.h"
#include "raylib.h"

namespace platformer2d {

LevelEditor::LevelEditor() : background_color_(SKYBLUE), asset_manager_{} {
  // Setup Window
  InitWindow(kScreenWidth, kScreenHeight, "Level Editor");
  SetTargetFPS(kTargetFPS);
}

LevelEditor::~LevelEditor() { CloseWindow(); }

void LevelEditor::update() {
  // Update the level editor
}

void LevelEditor::draw() const {
  // Draw the level editor
  BeginDrawing();
  ClearBackground(background_color_);
  DrawText("Level Editor", 10, 10, 20, background_color_);
  EndDrawing();
}

}  // namespace platformer2d
