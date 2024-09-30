#pragma once

#include "scenes/level_scene.h"

namespace platformer2d {

class Game {
 public:
  Game(int width, int height);
  ~Game();

  // Deltete copy consturctors
  Game(const Game& other) = delete;
  Game& operator=(const Game& other) = delete;

  // Delete move constructors
  Game(Game&& other) = delete;
  Game operator=(Game&& other) = delete;

  // Public methods
  void update();
  void draw() const;

 private:
  const int screen_width_;
  const int screen_height_;
  Color background_color_;
  LevelScene level_;
};

}  // namespace platformer2d
