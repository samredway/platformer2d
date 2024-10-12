#include <functional>
#include <vector>

#include "level_editor/tile_map.h"
#include "managers/asset_manager.h"
#include "raylib.h"

namespace platformer2d {

TileMap::TileMap(size_t max_tiles_x, size_t max_tiles_y,
                 AssetManager& asset_manager)
    : max_tiles_x_(max_tiles_x),
      max_tiles_y_(max_tiles_y),
      tiles_{max_tiles_y_, std::vector<Tile>(max_tiles_x_)},
      asset_manager_(asset_manager) {}

const TilesVec& TileMap::getTiles() const { return tiles_; }

bool TileMap::addTile(size_t tile_x, size_t tile_y, float pos_x, float pos_y,
                      std::string texture_name) {
  if (!isInBounds(tile_x, tile_y)) {
    return false;
  }
  tiles_[tile_y][tile_x].texture_name = texture_name;
  tiles_[tile_y][tile_x].x = pos_x;
  tiles_[tile_y][tile_x].y = pos_y;
  return true;
}

std::optional<std::reference_wrapper<const Tile>> TileMap::getTile(
    size_t tile_x, size_t tile_y) const {
  if (!isInBounds(tile_x, tile_y)) {
    return std::nullopt;
  }
  return std::ref(tiles_[tile_y][tile_x]);
}

void TileMap::draw() const {
  // Draw in the placed tiles
  for (const auto& row : getTiles()) {
    for (const auto& tile : row) {
      if (tile.texture_name != "") {
        DrawTexture(asset_manager_.getTexture(tile.texture_name), tile.x,
                    tile.y, WHITE);
      }
    }
  }
}

bool TileMap::isInBounds(size_t tile_x, size_t tile_y) const {
  if (tile_x > max_tiles_x_ || tile_y > max_tiles_y_) {
    return false;
  }
  return true;
}

}  // namespace platformer2d
