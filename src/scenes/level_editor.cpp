#include "scenes/level_editor.h"

#include "constants.h"
#include "managers/asset_manager.h"
#include "managers/input_manager.h"
#include "raylib.h"
#include "scenes/scene.h"
#include "scenes/tile_map.h"

namespace platformer2d {

// Forward declare free helpers
void drawGrid();

LevelEditor::LevelEditor(AssetManager& asset_manager,
                         InputManager& input_manager, float width, float height)
    : Scene("editor", SKYBLUE, asset_manager, input_manager),
      width_(width),
      height_(height),
      tile_map_{} {}

void LevelEditor::init() {
  // Load in all the Tile textures
  asset_manager_.loadTexture("winter_ground_0",
                             "assets/winter_ground/ground0.png");
  asset_manager_.loadTexture("winter_ground_1",
                             "assets/winter_ground/ground1.png");
  asset_manager_.loadTexture("winter_ground_2",
                             "assets/winter_ground/ground2.png");
  asset_manager_.loadTexture("winter_ground_3",
                             "assets/winter_ground/ground3.png");
  asset_manager_.loadTexture("winter_ground_4",
                             "assets/winter_ground/ground4.png");
  asset_manager_.loadTexture("winter_ground_5",
                             "assets/winter_ground/ground5.png");
  asset_manager_.loadTexture("winter_ground_6",
                             "assets/winter_ground/ground6.png");
  asset_manager_.loadTexture("winter_ground_7",
                             "assets/winter_ground/ground7.png");
  asset_manager_.loadTexture("winter_ground_8",
                             "assets/winter_ground/ground8.png");
  asset_manager_.loadTexture("winter_ground_9",
                             "assets/winter_ground/ground9.png");
  asset_manager_.loadTexture("winter_groundIce1",
                             "assets/winter_ground/groundIce1.png");
  asset_manager_.loadTexture("winter_groundIce2",
                             "assets/winter_ground/groundIce2.png");
  asset_manager_.loadTexture("winter_groundIce3",
                             "assets/winter_ground/groundIce3.png");
  asset_manager_.loadTexture("winter_groundl",
                             "assets/winter_ground/groundl.png");
  asset_manager_.loadTexture("winter_groundr",
                             "assets/winter_ground/groundr.png");
  asset_manager_.loadTexture("winter_ice", "assets/winter_ground/ice.png");

  // TODO load in character sprites and select single animation frame
  // to place them
}

void LevelEditor::update() {
  // Update the level editor
  handleInput();
}

void LevelEditor::handleInput() {
  // Handle input for the level editor
  if (input_manager_.mouseClicked()) {
    int tile_count_x = (input_manager_.getMousePositionX() / kTileSize);
    int tile_count_y = (input_manager_.getMousePositionY() / kTileSize);
    tile_map_.addTile(tile_count_x, tile_count_y);
  }
}

void LevelEditor::draw() const {
  ClearBackground(background_color_);
  DrawText("Level Editor", 10, 10, 20, BLACK);

  // Draw the tile map as a grid
  drawGrid();

  // Draw in the placed tiles
  drawTileMap();
}

void LevelEditor::drawTileMap() const {
  // Draw in the placed tiles
  for (const auto& row : tile_map_.getTiles()) {
    for (const auto& tile : row) {
      if (tile.texture_name != "") {
        DrawTexture(asset_manager_.getTexture(tile.texture_name), tile.x,
                    tile.y, WHITE);
      }
    }
  }
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
