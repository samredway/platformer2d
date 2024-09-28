#pragma once

#include <string>
#include <unordered_map>

#include "components.h"
#include "handlers/asset_handler.h"
#include "handlers/input_handler.h"
#include "scenes/scene.h"
#include "systems/animation_state_system.h"
#include "systems/animation_system.h"
#include "systems/physics_system.h"

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
  AnimationSystem animation_system_;
  // Owned component maps
  std::unordered_map<std::string, PositionComponent> position_components_;
  std::unordered_map<std::string, CollisionComponent> collision_components_;
  std::unordered_map<std::string, MovementComponent> movement_components_;
  std::unordered_map<std::string, RenderComponent> render_components_;
  std::unordered_map<std::string, AnimationComponent> animation_components_;
  AnimationStateSystem animation_state_system_;
};

}  // namespace platformer2d
