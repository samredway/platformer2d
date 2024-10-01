#include "scenes/level_editor.h"

#include "constants.h"
#include "managers/asset_manager.h"
#include "managers/input_manager.h"
#include "raylib.h"
#include "scenes/scene.h"
#include "scenes/tile_map.h"

namespace platformer2d {

// Forwatd declare free helpers
void drawGrid();

LevelEditor::LevelEditor(AssetManager& asset_manager,
                         InputManager& input_manager, float width, float height)
    : Scene("editor", SKYBLUE, asset_manager, input_manager),
      width_(width),
      height_(height),
      tile_map_{} {}

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

void LevelEditor::handleInput() {
  // Handle input for the level editor
}

void LevelEditor::draw() const {
  ClearBackground(background_color_);
  DrawText("Level Editor", 10, 10, 20, BLACK);

  // Draw the tile map as a grid
  drawGrid();
  // Draw in the placed tiles
  tile_map_.draw();
}

// Free helper Methods
void LevelEditor::drawGrid() const {
  for (int x = 0; x < width_; x += kTileSize) {
    DrawLine(x, 0, x, height_, BLACK);
  }
  for (int y = 0; y < height_; y += kTileSize) {
    DrawLine(0, y, width_, y, BLACK);
  }
}

}  // namespace platformer2d
