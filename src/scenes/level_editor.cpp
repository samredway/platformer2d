#include "scenes/level_editor.h"

#include <fstream>

#include "constants.h"
#include "json.hpp"
#include "macros.h"
#include "managers/asset_manager.h"
#include "managers/input_manager.h"
#include "raylib.h"
#include "scenes/scene.h"

namespace platformer2d {

constexpr size_t kNumTilesX = (size_t)(kScreenWidth / kTileSize);
constexpr size_t kNumTilesY = (size_t)(kScreenHeight / kTileSize);

// Forward declare free helpers
void drawGrid();

LevelEditor::LevelEditor(AssetManager& asset_manager,
                         InputManager& input_manager)
    : Scene("editor", SKYBLUE, asset_manager, input_manager),
      tile_map_{kNumTilesX, kNumTilesY, asset_manager},
      tile_picker_{asset_manager} {}

void LevelEditor::init() { tile_picker_.init(); }

void LevelEditor::update() {
  // Update the level editor
  handleInput();
}

void LevelEditor::handleInput() {
  // Handle mouse input for the level editor
  if (input_manager_.mouseClicked()) {
    const size_t tile_count_x =
        (input_manager_.getMousePositionX() / kTileSize);
    const size_t tile_count_y =
        (input_manager_.getMousePositionY() / kTileSize);
    // Check if the tile is on the tile editor grid
    if (tile_count_x < tile_map_.getTiles()[0].size() &&
        tile_count_y < tile_map_.getTiles().size()) {
      const float pos_x = tile_count_x * kTileSize;
      const float pos_y = tile_count_y * kTileSize;
      bool added = tile_map_.addTile(tile_count_x, tile_count_y, pos_x, pos_y,
                                     tile_picker_.getCurrentTextureName());
      if (!added) {
        PANIC("Tile at " << tile_count_x << ", " << tile_count_y
                         << " is out of bounds");
      }
    } else {
      tile_picker_.setCurrentTextureName(input_manager_.getMousePositionX(),
                                         input_manager_.getMousePositionY());
    }
  }

  // Handle keyboard input for the level editor
  if (input_manager_.isSPressed()) {
    save();
  }
}

void LevelEditor::draw() const {
  ClearBackground(background_color_);
  DrawText("Level Editor e to toggle mode and s to save", 10, 10, 15, BLACK);

  // Draw the tile map as a grid
  drawGrid();

  // Draw in the placed tiles
  tile_map_.draw();

  // Draw the tile picker
  tile_picker_.draw();
}

void LevelEditor::save() const {
  // convert tile map to json using nlohmann
  nlohmann::json json;
  json["tile_map"] = tile_map_.toJson();
  std::ofstream file("assets/levels/level_editor.json");
  if (!file.is_open()) {
    PANIC("Failed to open file for saving");
  }
  file << json;
  file.close();
  DLOG("Saved level to assets/levels/level_editor.json");
}

// Free helper Methods
void drawGrid() {
  for (int x = 0; x < kScreenWidth; x += kTileSize) {
    DrawLine(x, 0, x, kScreenHeight, BLACK);
  }
  for (int y = 0; y < kScreenHeight; y += kTileSize) {
    DrawLine(0, y, kScreenWidth, y, BLACK);
  }
}

}  // namespace platformer2d
