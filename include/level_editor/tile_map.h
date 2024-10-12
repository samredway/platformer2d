#pragma once

#include <cstddef>
#include <functional>
#include <optional>
#include <vector>

#include "level_editor/tile.h"
#include "managers/asset_manager.h"

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
  TileMap(size_t max_tiles_x, size_t max_tiles_y, AssetManager& asset_manager);
  // Return false in case of out of bounds
  bool addTile(size_t tile_x, size_t tile_y, float pos_x, float pos_y,
               std::string texture_name);
  std::optional<std::reference_wrapper<const Tile>> getTile(
      size_t tile_x, size_t tile_y) const;
  const TilesVec& getTiles() const;
  void draw() const;

  size_t getMaxTilesX() { return max_tiles_x_; }

  size_t getMaxTilesY() { return max_tiles_y_; }

 private:
  bool isInBounds(size_t tile_x, size_t tile_y) const;

  size_t max_tiles_x_;
  size_t max_tiles_y_;
  TilesVec tiles_;
  AssetManager& asset_manager_;
};

}  // namespace platformer2d
