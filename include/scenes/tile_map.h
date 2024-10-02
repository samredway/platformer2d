#pragma once

#include <array>
#include <cstddef>

#include "components/render_component.h"
#include "constants.h"

namespace platformer2d {

class TileMap {
 public:
  TileMap();
  // TODO handle error if tile out of bounds?
  void addTile(int tile_count_x, int tile_count_y);
  void removeTile();
  void draw() const;

 private:
  std::array<std::array<RenderComponent, (size_t)(kScreenWidth / kTileSize)>,
             (size_t)(kScreenHeight / kTileSize)>
      tiles_;
};

}  // namespace platformer2d
