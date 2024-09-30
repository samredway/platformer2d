#pragma once

#include "handlers/asset_handler.h"
#include "raylib.h"

namespace platformer2d {

class LevelEditor {
 public:
  LevelEditor();
  ~LevelEditor();

  // Delete copy operations
  LevelEditor(const LevelEditor&) = delete;
  LevelEditor& operator=(const LevelEditor&) = delete;

  // Delete move operations
  LevelEditor(LevelEditor&&) = delete;
  LevelEditor& operator=(LevelEditor&&) = delete;

  // public methods
  void update();
  void draw() const;

 private:
  Color background_color_;
  AssetManager asset_manager_;
};

}  // namespace platformer2d
