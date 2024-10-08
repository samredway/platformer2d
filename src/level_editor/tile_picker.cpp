#include "level_editor/tile_picker.h"
#include "macros.h"
#include "raylib.h"

namespace platformer2d {

TilePicker::TilePicker(AssetManager& asset_manager)
    : asset_manager_{asset_manager} {
  auto texture_it = asset_manager_.getTextures().begin();
  auto texture_end = asset_manager_.getTextures().end();

  DLOG("Starting Tile Picker Constructor");

  while (texture_it != texture_end) {
    DLOG("Texuture id: " << texture_it->first);
    ++texture_it;
  }

  // bool should_break = false;
  // for (size_t i = 0; i < tile_map_.size() && !should_break; ++i) {
  //   for (size_t j = 0; j < tile_map_[i].size() && !should_break; ++j) {
  //     if (texture_it == texture_end) {
  //       DLOG("Texture iterator out of range");
  //       should_break = true;
  //       break;
  //     }
  //     tile_map_[i][j] = texture_it->first;
  //     ++texture_it;

  //     DLOG(tile_map_[i][j] << " written in tile picker tile map at loc " << i
  //                          << " " << j);
  //   }
  // }
}

void TilePicker::draw() const {
  // Draw title
  DrawText("Tile Picker", (int)left_border_x + 10, (int)top_border_y - 30, 15,
           BLACK);
  // Borders for the tile picker
  DrawLineEx(Vector2{left_border_x, top_border_y},
             Vector2{right_border_x, top_border_y}, 2, BLACK);
  DrawLineEx(Vector2{left_border_x, bottom_border_y},
             Vector2{right_border_x, bottom_border_y}, 2, BLACK);
  DrawLineEx(Vector2{left_border_x, kScreenHeight}, Vector2{left_border_x, 0},
             2, BLACK);
  DrawLineEx(Vector2{right_border_x - 1, kScreenHeight},
             Vector2{right_border_x - 1, 0}, 2, BLACK);
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
