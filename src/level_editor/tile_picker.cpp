#include "constants.h"
#include "level_editor/tile_picker.h"
#include "debug.h"
#include "raylib.h"

namespace platformer2d {

constexpr size_t kPickerNumTilesX = kTilePickerWidth / kTileSize;
constexpr size_t kPickerNumTilesY =
    (kScreenHeight - (2 * kTileSize)) / kTileSize;

TilePicker::TilePicker(AssetManager& asset_manager)
    : asset_manager_{asset_manager},
      current_texture_name_("tile_winter_ice"),
      tile_map_{kPickerNumTilesX, kPickerNumTilesY, asset_manager} {}

void TilePicker::init() {
  // Loop through assets and write each tile to the tilemap
  // For now we will ignore sprites
  // Note the first one is black so we can use it as a delete
  // tile thus we start with x=1 not 0
  size_t count_x = 1;
  size_t count_y = 0;
  for (auto& asset_it : asset_manager_.getTextures()) {
    auto& texture_name = asset_it.first;
    // Only load tiles in right now. Ignore sprites
    if (!texture_name.starts_with("tile_")) {
      continue;
    }
    const float pos_x = (count_x * kTileSize) + left_border_x;
    const float pos_y = (count_y * kTileSize) + top_border_y;
    bool added =
        tile_map_.addTile(count_x, count_y, pos_x, pos_y, texture_name);
    if (!added) {
      PANIC("Attempted to place tile out of bounds");
    }
    ++count_x;
    if (count_x == tile_map_.getMaxTilesX()) {
      count_x = 0;
      ++count_y;
      if (count_y == tile_map_.getMaxTilesY()) {
        count_y = 0;
      }
    }
  }
}

void TilePicker::draw() const {
  // Draw title
  DrawText("Tile Picker", (int)left_border_x + 10, (int)top_border_y - 30, 15,
           BLACK);
  // Borders for the tile picker
  DrawLineEx(Vector2{left_border_x, top_border_y},
             Vector2{right_border_x, top_border_y}, 2, BLACK);
  DrawLineEx(Vector2{left_border_x, bottom_border_y},
             Vector2{right_border_x, bottom_border_y}, 2, BLACK);
  DrawLineEx(Vector2{left_border_x, kScreenHeight}, Vector2{left_border_x, 0},
             2, BLACK);
  DrawLineEx(Vector2{right_border_x - 1, kScreenHeight},
             Vector2{right_border_x - 1, 0}, 2, BLACK);

  tile_map_.draw();
}

void TilePicker::setCurrentTextureName(int mouse_x, int mouse_y) {
  DLOG("Mouse x: " << mouse_x << " Mouse y: " << mouse_y);
  // Find the position in tile map
  size_t count_x = (mouse_x - left_border_x) / kTileSize;
  size_t count_y = (mouse_y - top_border_y) / kTileSize;
  auto tile = tile_map_.getTile(count_x, count_y);
  if (!tile) {
    PANIC("Out of bounds error selecting tile");
  }
  DLOG("Setting current texture name: " << tile->get().texture_name);
  current_texture_name_ = tile->get().texture_name;
}

std::string TilePicker::getCurrentTextureName() const {
  return current_texture_name_;
}

}  // namespace platformer2d
