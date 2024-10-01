#include "scenes/tile_map.h"

#include "components/render_component.h"

namespace platformer2d {

TileMap::TileMap() {
  // Initialize all tiles with empty RenderComponents
  for (auto& row : tiles_) {
    for (auto& tile : row) {
      tile = RenderComponent{};  // Default constructor creates an empty
                                 // RenderComponent
    }
  }
}

void TileMap::addTile() {}

void TileMap::removeTile() {}

void TileMap::draw() const {}

}  // namespace platformer2d
