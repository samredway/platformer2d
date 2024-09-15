#pragma once

#include <unordered_map>

#include "components/physics_component.h"
#include "components/render_component.h"

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
  std::unordered_map<std::string, PhysicsComponent> physics_components_;
  std::unordered_map<std::string, RenderComponent> render_components_;
};

}  // namespace platformer2d
