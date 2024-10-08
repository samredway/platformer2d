#include "scenes/level_scene.h"

#include <format>
#include <string>

#include "components/animation_component.h"
#include "components/movement_component.h"
#include "constants.h"
#include "raylib.h"
#include "scenes/scene.h"

namespace platformer2d {

const std::string playerTag{"player"};

LevelScene::LevelScene(AssetManager& asset_manager, InputManager& input_manager,
                       const float width, const float height)
    : Scene("level", SKYBLUE, asset_manager, input_manager),
      width_(width),
      height_(height),
      physics_(movement_components_, position_components_,
               collision_components_),
      animation_system_(animation_components_, position_components_,
                        movement_components_, asset_manager_),
      animation_state_system_(animation_components_, movement_components_),
      render_system_(position_components_, render_components_, asset_manager_) {
}

void LevelScene::init() {
  // Load textures
  asset_manager_.loadTexture("winter_ground_1",
                             "assets/winter_ground/ground1.png", 50, 50);
  asset_manager_.loadTexture("pink_monster_idle",
                             "assets/Pink_Monster_Idle_4.png");
  asset_manager_.loadTexture("pink_monster_run",
                             "assets/Pink_Monster_Run_6.png");
  asset_manager_.loadTexture("winter_ice", "assets/winter_ground/ice.png", 50,
                             50);

  // Initialise player components
  initPlayer();

  // Floor tiles
  for (int i = 0; i < 8; i++) {
    const std::string tileTag{std::format("tile{}", i)};
    const float x{(width_ / 2.0f) - ((4.0f - i) * kTileSize)};
    position_components_.emplace(
        tileTag, PositionComponent{tileTag, x, (float)height_ - kTileSize});
    render_components_.emplace(tileTag,
                               RenderComponent{tileTag, "winter_ground_1"});
    collision_components_.emplace(
        tileTag, CollisionComponent{tileTag, kTileSize, kTileSize});
  }

  // Central tile to do x collision on
  const std::string tileTag{std::format("tile8")};
  const float x{(width_ / 2.0f) - kTileSize / 2};
  position_components_.emplace(
      tileTag, PositionComponent{tileTag, x, (float)height_ - kTileSize * 2});
  render_components_.emplace(tileTag, RenderComponent{tileTag, "winter_ice"});
  collision_components_.emplace(
      tileTag, CollisionComponent{tileTag, kTileSize, kTileSize});
}

void LevelScene::initPlayer() {
  position_components_.emplace(
      playerTag,
      PositionComponent{playerTag, (float)width_ / 2, (float)height_ / 2});
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
  DrawText("Running in debug mode, press 'E' to toggle editor", 10, 10, 15,
           BLACK);
  DrawText(TextFormat("FPS: %i", GetFPS()), 10, 25, 10, BLACK);
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
