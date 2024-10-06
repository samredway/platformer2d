#pragma once

#include "level_editor/tile_map.h"
#include "level_editor/tile_picker.h"
#include "managers/asset_manager.h"
#include "managers/input_manager.h"
#include "scenes/scene.h"

namespace platformer2d {

class LevelEditor : public Scene {
 public:
  LevelEditor(AssetManager& asset_manager, InputManager& input_manager);

  void init() override;
  void update() override;
  void draw() const override;

 private:
  TileMap tile_map_;
  TilePicker tile_picker_;

  // Private methods
  void handleInput() override;
  void drawTileMap() const;
};

}  // namespace platformer2d
