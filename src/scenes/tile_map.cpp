#include "components/render_component.h"
#include "macros.h"
#include "scenes/tile_map.h"

namespace platformer2d {

/**
 *  Internal model of the tile map is an array of arrays
 *  [
 *    [   screen width / kTileSize num columns  ],
 *    ... sreeen height / kTileSize num rows
 *  ]
 *
 */

TileMap::TileMap() {
  // Initialize all tiles with empty RenderComponents
  for (auto& row : tiles_) {
    for (auto& tile : row) {
      tile = RenderComponent{};  // Default constructor creates an empty
                                 // RenderComponent
    }
  }
}

void TileMap::addTile(int tile_count_x, int tile_count_y) {
  DLOG("Adding tile with tile_count_x " << tile_count_x << " and tile_count_y "
                                        << tile_count_y);
  tiles_[tile_count_y][tile_count_x].texture_name = "winter_ground_1";
}

void TileMap::removeTile() {}

void TileMap::draw() const {}

}  // namespace platformer2d
