#include "constants.h"
#include "level_editor/tile_map.h"
#include "macros.h"
#include "managers/asset_manager.h"
#include "managers/input_manager.h"
#include "raylib.h"
#include "scenes/level_editor.h"
#include "scenes/scene.h"

namespace platformer2d {

constexpr size_t kNumTilesX = (size_t)(kScreenWidth / kTileSize);
constexpr size_t kNumTilesY = (size_t)(kScreenHeight / kTileSize);
//
// Forward declare free helpers
void drawGrid();

LevelEditor::LevelEditor(AssetManager& asset_manager,
                         InputManager& input_manager)
    : Scene("editor", SKYBLUE, asset_manager, input_manager),
      tile_map_{kNumTilesX, kNumTilesY, asset_manager},
      tile_picker_{asset_manager} {}

void LevelEditor::init() {
  // Load in all the Tile textures
  asset_manager_.loadTexture("tile_winter_ground_0",
                             "assets/winter_ground/ground0.png");
  asset_manager_.loadTexture("tile_winter_ground_1",
                             "assets/winter_ground/ground1.png");
  asset_manager_.loadTexture("tile_winter_ground_2",
                             "assets/winter_ground/ground2.png");
  asset_manager_.loadTexture("tile_winter_ground_3",
                             "assets/winter_ground/ground3.png");
  asset_manager_.loadTexture("tile_winter_ground_4",
                             "assets/winter_ground/ground4.png");
  asset_manager_.loadTexture("tile_winter_ground_5",
                             "assets/winter_ground/ground5.png");
  asset_manager_.loadTexture("tile_winter_ground_6",
                             "assets/winter_ground/ground6.png");
  asset_manager_.loadTexture("tile_winter_ground_7",
                             "assets/winter_ground/ground7.png");
  asset_manager_.loadTexture("tile_winter_ground_8",
                             "assets/winter_ground/ground8.png");
  asset_manager_.loadTexture("tile_winter_ground_9",
                             "assets/winter_ground/ground9.png");
  asset_manager_.loadTexture("tile_winter_groundIce1",
                             "assets/winter_ground/groundIce1.png");
  asset_manager_.loadTexture("tile_winter_groundIce2",
                             "assets/winter_ground/groundIce2.png");
  asset_manager_.loadTexture("tile_winter_groundIce3",
                             "assets/winter_ground/groundIce3.png");
  asset_manager_.loadTexture("tile_winter_groundl",
                             "assets/winter_ground/groundl.png");
  asset_manager_.loadTexture("tile_winter_groundr",
                             "assets/winter_ground/groundr.png");
  asset_manager_.loadTexture("tile_winter_ice", "assets/winter_ground/ice.png");

  // TODO load in character sprites and select single animation frame
  // to place them

  tile_picker_.init();
}

void LevelEditor::update() {
  // Update the level editor
  handleInput();
}

void LevelEditor::handleInput() {
  // Handle input for the level editor
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
}

void LevelEditor::draw() const {
  ClearBackground(background_color_);
  DrawText("Level Editor", 10, 10, 15, BLACK);

  // Draw the tile map as a grid
  drawGrid();

  // Draw in the placed tiles
  tile_map_.draw();

  // Draw the tile picker
  tile_picker_.draw();
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
