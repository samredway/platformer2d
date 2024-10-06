#include "level_editor/tile_picker.h"

#include "macros.h"

namespace platformer2d {

TilePicker::TilePicker(AssetManager& asset_manager)
    : asset_manager_{asset_manager} {}

void TilePicker::draw() const {
  // TODO draw a border for the tile picker
  // border should have space at the top for a title
  // And space at the bottom for left and right arrow buttons
  // which will allow us to scroll through the textures

  // TODO draw each texture in the asset manger to a grid in the tile picker
  // Tile picker will proably have to be scrollable so that it can show all
  // textures
  for (const auto& pair : asset_manager_.getTextures()) {
    DLOG(pair.first);
  }
}

// TODO
void TilePicker::setCurrentTextureName(int mouse_x, int mouse_y) {
  DLOG("Mouse x: " << mouse_x << " Mouse y: " << mouse_y);
}

// TODO
std::string TilePicker::getCurrentTextureName() const {
  return "winter_ground_1";
}

}  // namespace platformer2d
