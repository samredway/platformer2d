#include "scenes/tile_map.h"

#include "macros.h"

namespace platformer2d {

/**
 *  Internal model of the tile map is an array of arrays
 *  [
 *    [   screen width / kTileSize num columns  ],
 *    ... sreeen height / kTileSize num rows
 *  ]
 *
 */

const TilesArray& TileMap::getTiles() const { return tiles_; }

void TileMap::addTile(int tile_count_x, int tile_count_y) {
  DLOG("Adding tile with tile_count_x " << tile_count_x << " and tile_count_y "
                                        << tile_count_y);
  const float x_pos = tile_count_x * kTileSize;
  const float y_pos = tile_count_y * kTileSize;
  tiles_[tile_count_y][tile_count_x].texture_name = "winter_ground_1";
  tiles_[tile_count_y][tile_count_x].x = x_pos;
  tiles_[tile_count_y][tile_count_x].y = y_pos;
}

void TileMap::removeTile() {}

}  // namespace platformer2d
