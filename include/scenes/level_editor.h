#pragma once

#include "managers/asset_manager.h"
#include "managers/input_manager.h"
#include "raylib.h"
#include "scenes/scene.h"
#include "tile_map.h"

namespace platformer2d {

class LevelEditor : public Scene {
 public:
  LevelEditor(AssetManager& asset_manager, InputManager& input_manager,
              float width, float height);

  void init() override;
  void update() override;
  void draw() const override;

 private:
  float width_;
  float height_;
  TileMap tile_map_;

  // Private methods
  void handleInput() override;
  void drawGrid() const;
};

}  // namespace platformer2d
