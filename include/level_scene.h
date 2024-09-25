#pragma once

#include <string>
#include <unordered_map>

#include "asset_manager.h"
#include "components.h"
#include "input_handler.h"
#include "physics_system.h"
#include "scene.h"

namespace platformer2d {

class LevelScene : Scene {
 public:
  LevelScene(const float width, const float height);
  void draw() const override;
  void update() override;
  void init();

 private:
  void handleInput();
  void processRendering() const;

  const float width_;
  const float height_;

  // Owned systems
  AssetManager assets_;
  InputHandler input_handler_;
  PhysicsSystem physics_;

  // Owned component maps
  std::unordered_map<std::string, PositionComponent> position_components_;
  std::unordered_map<std::string, CollisionComponent> collision_components_;
  std::unordered_map<std::string, MovementComponent> movement_components_;
  std::unordered_map<std::string, RenderComponent> render_components_;
};

}  // namespace platformer2d
