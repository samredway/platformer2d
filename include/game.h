#pragma once

#include <string>
#include <unordered_map>

#include "components.h"
#include "input_handler.h"

namespace platformer2d {

class Game {
 public:
  Game(int width, int height);
  ~Game();
  void update();
  void draw() const;

 private:
  void handleInput();
  void processRendering() const;
  void processPhysics();
  void processPhysicsX(float delta_time, MovementComponent& movement);
  void processPhysicsY(float delta_time, MovementComponent& movement);

  const int screen_width_;
  const int screen_height_;
  InputHandler input_handler_;

  // Systems are just collections of components right now with their
  // own processor method (these probably get moved out to scenes or
  // systems later)
  std::unordered_map<std::string, PositionComponent> position_components_;
  std::unordered_map<std::string, CollisionComponent> collision_components_;
  std::unordered_map<std::string, MovementComponent> movement_components_;
  std::unordered_map<std::string, RenderComponent> render_components_;
};

}  // namespace platformer2d
