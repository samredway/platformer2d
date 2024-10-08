#include <vector>

#include "constants.h"
#include "level_editor/tile_map.h"

namespace platformer2d {

TileMap::TileMap(size_t max_tiles_x, size_t max_tiles_y)
    : max_tiles_x_(max_tiles_x),
      max_tiles_y_(max_tiles_y),
      tiles_{max_tiles_y_, std::vector<Tile>(max_tiles_x_)} {}

const TilesVec& TileMap::getTiles() const { return tiles_; }

bool TileMap::addTile(size_t tile_count_x, size_t tile_count_y,
                      std::string texture_name) {
  if (tile_count_x > max_tiles_x_ || tile_count_y > max_tiles_y_) {
    return false;
  }
  const float x_pos = tile_count_x * kTileSize;
  const float y_pos = tile_count_y * kTileSize;
  tiles_[tile_count_y][tile_count_x].texture_name = texture_name;
  tiles_[tile_count_y][tile_count_x].x = x_pos;
  tiles_[tile_count_y][tile_count_x].y = y_pos;
  return true;
}

void TileMap::removeTile() {}

}  // namespace platformer2d
