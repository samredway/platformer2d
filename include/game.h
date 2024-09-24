#pragma once

#include "level_scene.h"

namespace platformer2d {

class Game {
 public:
  Game(int width, int height);
  ~Game();
  void update();
  void draw() const;

 private:
  const int screen_width_;
  const int screen_height_;
  LevelScene level_;
};

}  // namespace platformer2d
