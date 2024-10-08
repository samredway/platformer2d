#pragma once

#include "constants.h"
#include "managers/asset_manager.h"
#include "tile_map.h"

namespace platformer2d {

constexpr float top_border_y{kTileSize};
constexpr float bottom_border_y{kScreenHeight - kTileSize};
constexpr float left_border_x{kScreenWidth};
constexpr float right_border_x{kScreenWidth + kTilePickerWidth};

class TilePicker {
 public:
  TilePicker(AssetManager& asset_manager);
  void draw() const;
  void setCurrentTextureName(int mouse_x, int mouse_y);
  std::string getCurrentTextureName() const;

  // Fill the tile map with all tiles and sprites.
  // NOTE this only contains the number of textures that can fit in the
  // screen space at the moment. Later we will need to figure out how
  // to show more
  void init();

 private:
  const AssetManager& asset_manager_;
  std::string current_texture_name_;
  TileMap tile_map_;
};

}  // namespace platformer2d
