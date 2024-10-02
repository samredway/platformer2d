#include "constants.h"
#include "managers/asset_manager.h"
#include "managers/input_manager.h"
#include "raylib.h"
#include "scenes/level_editor.h"
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
  asset_manager_.loadTexture("winter_ground_0", "assets/ground0.png");
  asset_manager_.loadTexture("winter_ground_1", "assets/ground1.png");
  asset_manager_.loadTexture("winter_ground_2", "assets/ground2.png");
  asset_manager_.loadTexture("winter_ground_3", "assets/ground3.png");
  asset_manager_.loadTexture("winter_ground_4", "assets/ground4.png");
  asset_manager_.loadTexture("winter_ground_5", "assets/ground5.png");
  asset_manager_.loadTexture("winter_ground_6", "assets/ground6.png");
  asset_manager_.loadTexture("winter_ground_7", "assets/ground7.png");
  asset_manager_.loadTexture("winter_ground_8", "assets/ground8.png");
  asset_manager_.loadTexture("winter_ground_9", "assets/ground9.png");
  asset_manager_.loadTexture("winter_groundIce1", "assets/groundIce1.png");
  asset_manager_.loadTexture("winter_groundIce2", "assets/groundIce2.png");
  asset_manager_.loadTexture("winter_groundIce3", "assets/groundIce3.png");
  asset_manager_.loadTexture("winter_groundl", "assets/groundl.png");
  asset_manager_.loadTexture("winter_groundr", "assets/groundr.png");
  asset_manager_.loadTexture("winter_ice", "assets/ice.png");

  // TODO load in character sprites and select single animation frame
  // to place them

  asset_manager_.debugLogTextures();
}

void LevelEditor::update() {
  // Update the level editor
}

void LevelEditor::handleInput() {
  // Handle input for the level editor
  if (input_manager_.mouseClicked()) {
    int tile_count_x = (input_manager_.getMousePositionX() / kTileSize) + 1;
    int tile_count_y = (input_manager_.getMousePositionY() / kTileSize) + 1;
    tile_map_.addTile(tile_count_x, tile_count_y);
  }
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
