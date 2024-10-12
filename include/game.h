#pragma once

#include "scenes/scene.h"
#include <memory>

namespace platformer2d {

class Game {
 public:
  Game();
  ~Game();

  // Delete copy constructors
  Game(const Game& other) = delete;
  Game& operator=(const Game& other) = delete;

  // Delete move constructors
  Game(Game&& other) = delete;
  Game& operator=(Game&& other) = delete;

  // Public methods
  void update();
  void draw() const;

 private:
  InputManager input_manager_;
  AssetManager asset_manager_;
  std::unique_ptr<Scene> current_scene_;

  void handleInput();
  void setCurrentScene(std::unique_ptr<Scene> new_scene);
};

}  // namespace platformer2d
