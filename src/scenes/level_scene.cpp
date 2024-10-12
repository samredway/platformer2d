#include "scenes/level_scene.h"

#include <fstream>
#include <string>

#include "components/animation_component.h"
#include "components/movement_component.h"
#include "constants.h"
#include "json.hpp"
#include "raylib.h"
#include "scenes/scene.h"

namespace platformer2d {

const std::string playerTag{"player"};

LevelScene::LevelScene(AssetManager& asset_manager, InputManager& input_manager)
    : Scene("level", SKYBLUE, asset_manager, input_manager),
      physics_(movement_components_, position_components_,
               collision_components_),
      animation_system_(animation_components_, position_components_,
                        movement_components_, asset_manager_),
      animation_state_system_(animation_components_, movement_components_),
      render_system_(position_components_, render_components_, asset_manager_) {
}

void LevelScene::init() {
  initPlayer();
  loadLevelFromFile();
}

void LevelScene::loadLevelFromFile() {
  std::ifstream file{"assets/levels/level_editor.json"};
  if (!file.is_open()) {
    PANIC("Failed to open level file");
  }
  nlohmann::json level_json;
  file >> level_json;

  DLOG("Loaded level from file:\n" << level_json.dump(2));

  // Create components from the json objects
  for (const auto& tile_row : level_json["tiles"]) {
    for (const auto& tile : tile_row) {
      render_components_.emplace(
          tile["texture"], RenderComponent{tile["texture"], tile["texture"]});
      position_components_.emplace(
          tile["texture"],
          PositionComponent{tile["texture"], tile["x"], tile["y"]});
    }
  }
}

void LevelScene::initPlayer() {
  position_components_.emplace(
      playerTag, PositionComponent{playerTag, (float)kScreenWidth / 2,
                                   (float)kScreenHeight / 2});
  movement_components_.emplace(playerTag, MovementComponent{playerTag});
  collision_components_.emplace(playerTag,
                                CollisionComponent{playerTag, 20, 40, 10, 0});
  AnimationComponent player_animation{playerTag, 1.3f};
  player_animation.current_state = AnimationState::kIdle;
  player_animation.setStateToNumFrames(AnimationState::kIdle, 4);
  player_animation.setStateToNumFrames(AnimationState::kRunning, 6);
  player_animation.setStateToTextureName(AnimationState::kIdle,
                                         "pink_monster_idle");
  player_animation.setStateToTextureName(AnimationState::kRunning,
                                         "pink_monster_run");
  player_animation.setStateToAnimationFPS(AnimationState::kIdle, 0.4f);
  player_animation.setStateToAnimationFPS(AnimationState::kRunning, 0.1f);
  animation_components_.emplace(playerTag, player_animation);
}

void LevelScene::update() {
  handleInput();
  animation_state_system_.update();
  animation_system_.update();
  physics_.update();
}

void LevelScene::draw() const {
  ClearBackground(background_color_);

#ifndef NDEBUG
  // Draw some debug info
  DrawText("DEBUG mode press e to toggle editor", 10, 10, 15, BLACK);
#endif

  // Draw static components (Tiles)
  render_system_.draw();

  // Draw animations (Sprites)
  animation_system_.draw();
}

void LevelScene::handleInput() {
  MovementComponent& player_movement =
      getComponentOrPanic<MovementComponent>(movement_components_, playerTag);

  const float rate_acceleration{player_movement.walk_force /
                                player_movement.mass};

  // Accelerate in direction pressed.
  // If nothing pressed then decellerate unless at rest then stop
  if (input_manager_.isRight() && player_movement.is_grounded) {
    // Accelerate right
    player_movement.acceleration_x = rate_acceleration;
    player_movement.is_facing_right = true;
  } else if (input_manager_.isLeft() && player_movement.is_grounded) {
    // Accelerate left
    player_movement.acceleration_x = -rate_acceleration;
    player_movement.is_facing_right = false;
  } else {
    player_movement.acceleration_x = 0;
    // Artbitrary decelleration rate to mimic players own force in
    // slowing down
    if (player_movement.is_grounded) {
      player_movement.velocity_x *= 0.90;
    }
  }

  // Jump
  if (input_manager_.isSpace() && player_movement.is_grounded) {
    player_movement.acceleration_y = player_movement.jump_force;
  } else if (player_movement.acceleration_y > 0) {
    player_movement.acceleration_y = 0;
  }
}

}  // namespace platformer2d
