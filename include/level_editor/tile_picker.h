#pragma once

#include "managers/asset_manager.h"

namespace platformer2d {

class TilePicker {
 public:
  TilePicker(AssetManager& asset_manager);
  void draw() const;
  void setCurrentTextureName(int mouse_x, int mouse_y);
  std::string getCurrentTextureName() const;

 private:
  AssetManager& asset_manager_;
  std::string current_texture_name_;
};

}  // namespace platformer2d
