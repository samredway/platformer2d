#pragma once

#include "scenes/scene.h"

namespace platformer2d {

class Game {
 public:
  Game();
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
  InputManager input_manager_;
  AssetManager asset_manager_;
  std::unordered_map<std::string, std::unique_ptr<Scene>> scenes_;
  Scene* current_scene_ = nullptr;

  void handleInput();
  void setCurrentScene(const std::string& scene_name);
};

}  // namespace platformer2d
