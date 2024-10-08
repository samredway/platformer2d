#pragma once

#include <array>

#include "constants.h"
#include "managers/asset_manager.h"

namespace platformer2d {

constexpr float top_border_y{kTileSize};
constexpr float bottom_border_y{kScreenHeight - kTileSize};
constexpr float left_border_x{kScreenWidth};
constexpr float right_border_x{kScreenWidth + kTilePickerWidth};

// This is slightly different to the tile map in the level editor as
// only stores the texture name only gets written to once when the tile
// picker is created
typedef std::array<
    std::array<std::string,
               (size_t)((bottom_border_y - top_border_y) / kTileSize)>,
    (size_t)((right_border_x - left_border_x) / kTileSize)>
    TilePickerTileMap;

// NOTE this only shows exactly the number of textures that can fit in the
// screen space at the moment. Later we will need to make it able to either
// fit all the tiles in the asset manager or add a function to repopulate
// the tile picker with a subset of the tiles in the asset manager on some
// kind of scroll event

class TilePicker {
 public:
  TilePicker(AssetManager& asset_manager);
  void draw() const;
  void setCurrentTextureName(int mouse_x, int mouse_y);
  std::string getCurrentTextureName() const;

 private:
  const AssetManager& asset_manager_;
  std::string current_texture_name_;
  TilePickerTileMap tile_map_;
};

}  // namespace platformer2d
