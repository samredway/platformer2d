#pragma once

#include <string>
#include <unordered_map>

#include "components/animation_component.h"
#include "components/collision_component.h"
#include "components/movement_component.h"
#include "components/position_component.h"
#include "components/render_component.h"
#include "managers/asset_manager.h"
#include "managers/input_manager.h"
#include "scenes/scene.h"
#include "systems/animation_state_system.h"
#include "systems/animation_system.h"
#include "systems/physics_system.h"
#include "systems/render_system.h"

namespace platformer2d {

class LevelScene : public Scene {
 public:
  LevelScene(AssetManager& asset_manager, InputManager& input_manager);
  void draw() const override;
  void update() override;
  void init() override;

 private:
  void handleInput() override;
  void processRendering() const;
  void initPlayer();

  // Owned systems
  PhysicsSystem physics_;
  AnimationSystem animation_system_;
  AnimationStateSystem animation_state_system_;
  RenderSystem render_system_;

  // Owned component maps
  std::unordered_map<std::string, PositionComponent> position_components_;
  std::unordered_map<std::string, CollisionComponent> collision_components_;
  std::unordered_map<std::string, MovementComponent> movement_components_;
  std::unordered_map<std::string, RenderComponent> render_components_;
  std::unordered_map<std::string, AnimationComponent> animation_components_;
};

}  // namespace platformer2d
