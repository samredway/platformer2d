#pragma once

#include <cstddef>
#include <vector>

#include "level_editor/tile.h"

namespace platformer2d {

typedef std::vector<std::vector<Tile>> TilesVec;

/**
 *  Internal model of the tile map is an vector of vectors
 *  [
 *    [ max_tiles_x  ],
 *    ..., size max_tiles_y
 *  ]
 *
 */
class TileMap {
 public:
  TileMap(size_t max_tiles_x, size_t max_tiles_y);
  // Return false in case of out of bounds
  bool addTile(size_t tile_count_x, size_t tile_count_y,
               std::string texture_name);
  void removeTile();
  const TilesVec& getTiles() const;

 private:
  size_t max_tiles_x_;
  size_t max_tiles_y_;
  TilesVec tiles_;
};

}  // namespace platformer2d
