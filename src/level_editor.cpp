#include "constants.h"
#include "level_editor.h"
#include "raylib.h"

namespace platformer2d {

LevelEditor::LevelEditor() : background_color_(SKYBLUE), asset_manager_{} {
  // Setup Window
  InitWindow(kScreenWidth, kScreenHeight, "Level Editor");
  SetTargetFPS(kTargetFPS);
}

LevelEditor::~LevelEditor() { CloseWindow(); }

void LevelEditor::init() {
  // Load in all the Tile textures
  asset_manager_.loadTexture("winter_ground_0", "ground0.png");
  asset_manager_.loadTexture("winter_ground_1", "ground1.png");
  asset_manager_.loadTexture("winter_ground_2", "ground2.png");
  asset_manager_.loadTexture("winter_ground_3", "ground3.png");
  asset_manager_.loadTexture("winter_ground_4", "ground4.png");
  asset_manager_.loadTexture("winter_ground_5", "ground5.png");
  asset_manager_.loadTexture("winter_ground_6", "ground6.png");
  asset_manager_.loadTexture("winter_ground_7", "ground7.png");
  asset_manager_.loadTexture("winter_ground_8", "ground8.png");
  asset_manager_.loadTexture("winter_ground_9", "ground9.png");
  asset_manager_.loadTexture("winter_groundIce1", "groundIce1.png");
  asset_manager_.loadTexture("winter_groundIce2", "groundIce2.png");
  asset_manager_.loadTexture("winter_groundIce3", "groundIce3.png");
  asset_manager_.loadTexture("winter_groundl", "groundl.png");
  asset_manager_.loadTexture("winter_groundr", "groundr.png");
  asset_manager_.loadTexture("winter_ice", "ice.png");

  // TODO load in character sprites and select single animation frame
  // to place them
}

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
