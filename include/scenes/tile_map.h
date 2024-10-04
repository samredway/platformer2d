#pragma once

#include <array>
#include <cstddef>
#include <string>

#include "constants.h"

namespace platformer2d {

struct Tile {
  int x;
  int y;
  std::string texture_name;
};

constexpr size_t kNumTilesX = (size_t)(kScreenWidth / kTileSize);
constexpr size_t kNumTilesY = (size_t)(kScreenHeight / kTileSize);

typedef std::array<std::array<Tile, kNumTilesX>, kNumTilesY> TilesArray;

class TileMap {
 public:
  TileMap() = default;
  void addTile(int tile_count_x, int tile_count_y);
  void removeTile();
  const TilesArray& getTiles() const;

 private:
  TilesArray tiles_;
};

}  // namespace platformer2d
