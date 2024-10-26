#include "scenes/level_scene.h"

#include <fstream>
#include <string>

#include "components/animation_component.h"
#include "components/movement_component.h"
#include "constants.h"
#include "nlohmann/json.hpp"
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
  std::string level_file_path{"assets/levels/level_editor.json"};
  std::ifstream file{level_file_path};
  if (!file.is_open()) {
    DLOG("Failed to open level file! Loading empty level");
    return;
  }
  DLOG("Loading level from file: " << level_file_path);
  nlohmann::json level_json;
  file >> level_json;
  // Create components from the json objects
  int counter{0};
  for (const auto& tile_row : level_json["tile_map"]["tiles"]) {
    for (const auto& tile : tile_row) {
      if (tile["texture_name"] == "") {
        continue;
      }
      ++counter;
      const std::string tile_tag{"tile_" + std::to_string(counter)};
      render_components_.emplace(
          tile_tag, RenderComponent{tile_tag, tile["texture_name"]});
      position_components_.emplace(
          tile_tag, PositionComponent{tile_tag, tile["x"], tile["y"]});
      collision_components_.emplace(
          tile_tag, CollisionComponent{tile_tag, kTileSize, kTileSize, 0, 0});

      // This feels a bit hacky later on will want to be able to add
      // characteristics to the tile in the level editor or tile picker but now
      // just add directly here
      if (tile["texture_name"] == "tile_winter_ice") {
        MovementComponent mover{tile_tag};
        mover.mass = 20.0f;
        mover.friction_coefficient = 20.0f;
        mover.is_grounded = true;
        movement_components_.emplace(tile_tag, mover);
      }
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
  player_animation.setStateToAnimationFPS(AnimationState::kRunning, 0.4f);
  animation_components_.emplace(playerTag, player_animation);
}

void LevelScene::update() {
  handleInput();
  physics_.update();
  animation_state_system_.update();
  animation_system_.update();
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

  // If player is in air we still allow them some left right
  // movement cause ... game. But lets reduct it with some arbitrary value
  const float movement_speed_divisor =
      player_movement.is_grounded ? 1.0 : player_movement.air_movement_divisor;
  const float rate_acceleration{
      (player_movement.walk_force / player_movement.mass) /
      movement_speed_divisor};

  // Accelerate in direction pressed.
  // If nothing pressed then decellerate unless at rest then stop
  if (input_manager_.isRight()) {
    // Accelerate right
    player_movement.acceleration_x = rate_acceleration;
    player_movement.is_facing_right = true;
  } else if (input_manager_.isLeft()) {
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
