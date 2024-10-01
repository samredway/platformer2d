#pragma once

#include <array>
#include <cstddef>

#include "components/render_component.h"
#include "constants.h"

namespace platformer2d {

class TileMap {
 public:
  TileMap();
  void addTile();
  void removeTile();
  void draw() const;

 private:
  std::array<std::array<RenderComponent, (size_t)(kScreenWidth / kTileSize)>,
             (size_t)(kScreenHeight / kTileSize)>
      tiles_;
};

}  // namespace platformer2d
