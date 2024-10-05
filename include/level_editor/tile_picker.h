#pragma once

#include "managers/asset_manager.h"

namespace platformer2d {

class TilePicker {
 public:
  TilePicker(AssetManager& asset_manager);
  void draw() const;

 private:
  AssetManager& asset_manager_;
};

}  // namespace platformer2d
