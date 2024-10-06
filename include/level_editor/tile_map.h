#pragma once

#include <array>
#include <cstddef>

#include "constants.h"
#include "level_editor/tile.h"

namespace platformer2d {

constexpr size_t kNumTilesX = (size_t)(kScreenWidth / kTileSize);
constexpr size_t kNumTilesY = (size_t)(kScreenHeight / kTileSize);

typedef std::array<std::array<Tile, kNumTilesX>, kNumTilesY> TilesArray;

class TileMap {
 public:
  TileMap() = default;
  void addTile(int tile_count_x, int tile_count_y, std::string texture_name);
  void removeTile();
  const TilesArray& getTiles() const;

 private:
  TilesArray tiles_;
};

}  // namespace platformer2d
