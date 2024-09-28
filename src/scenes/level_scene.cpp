#include "scenes/level_scene.h"

#include <format>
#include <string>
#include <utility>

#include "components.h"
#include "raylib.h"

namespace platformer2d {

const std::string playerTag{"player"};

LevelScene::LevelScene(const float width, const float height)
    : width_(width),
      height_(height),
      assets_(),
      input_handler_(),
      physics_(movement_components_, position_components_,
               collision_components_),
      animation_system_(animation_components_, position_components_, assets_),
      animation_state_system_(animation_components_, movement_components_),
      render_system_(position_components_, render_components_, assets_) {}

void LevelScene::init() {
  // Load textures
  assets_.loadTexture("winter_ground_1", "assets/winter_ground/ground1.png");
  assets_.loadTexture("pink_monster_idle", "assets/Pink_Monster_Idle_4.png");
  assets_.loadTexture("pink_monster_run", "assets/Pink_Monster_Run_6.png");
  assets_.loadTexture("ice_block", "assets/winter_ground/ice.png");

  // Initialise player components
  initPlayer();

  // Floor tiles
  for (int i = 0; i < 8; i++) {
    const std::string tileTag{std::format("tile{}", i)};
    const float x{(width_ / 2.0f) - ((4.0f - i) * 40)};
    position_components_.emplace(
        tileTag, PositionComponent{x, (float)height_ - 40, 40, 40});
    render_components_.emplace(tileTag, RenderComponent{"winter_ground_1"});
    collision_components_.emplace(tileTag, CollisionComponent{});
  }

  // Central tile to do x collision on
  const std::string tileTag{std::format("tile8")};
  const float x{(width_ / 2.0f) - 20};
  position_components_.emplace(
      tileTag, PositionComponent{x, (float)height_ - 80, 40, 40});
  render_components_.emplace(tileTag, RenderComponent{"ice_block"});
  collision_components_.emplace(tileTag, CollisionComponent{});
}

void LevelScene::initPlayer() {
  position_components_.emplace(
      playerTag,
      PositionComponent{(float)width_ / 2, (float)height_ / 2, 40, 40});
  movement_components_.emplace(playerTag, MovementComponent{});
  collision_components_.emplace(playerTag, CollisionComponent{});
  AnimationComponent player_animation{};
  player_animation.current_state = AnimationState::kIdle;
  player_animation.state_to_num_frames_map[AnimationState::kIdle] = 4;
  player_animation.state_to_texture_name_map[AnimationState::kIdle] =
      "pink_monster_idle";
  player_animation.state_to_texture_name_map[AnimationState::kRunning] =
      "pink_monster_run";
  player_animation.state_to_num_frames_map[AnimationState::kRunning] = 6;
  player_animation.animation_fps[AnimationState::kIdle] = 0.4f;
  player_animation.animation_fps[AnimationState::kRunning] = 0.1f;
  animation_components_.emplace(playerTag, player_animation);
}

void LevelScene::update() {
  handleInput();
  animation_state_system_.update();
  animation_system_.update();
  physics_.update();
}

void LevelScene::draw() const {
  // Draw static components (Tiles)
  render_system_.draw();

  // Draw animations (Sprites)
  animation_system_.draw();
}

void LevelScene::handleInput() {
  // Would crash if player not defined somehow
  auto& player_movement{movement_components_.at(playerTag)};
  auto& position{position_components_.at(playerTag)};

  // Get the keyboard input
  input_handler_.getInput();

  const float rate_accleration{player_movement.walk_force /
                               player_movement.mass};

  // Accelerate in direction pressed.
  // If nothing pressed then decellerate unless at rest then stop
  if (input_handler_.isRight() && player_movement.is_grounded) {
    // Accelerate right
    player_movement.acceleration_x = rate_accleration;
    position.is_facing_right = true;
  } else if (input_handler_.isLeft() && player_movement.is_grounded) {
    // Accelerate left
    player_movement.acceleration_x = -rate_accleration;
    position.is_facing_right = false;
  } else {
    player_movement.acceleration_x = 0;
    // Artbitrary decelleration rate to mimic players own force in
    // slowing down
    if (player_movement.is_grounded) {
      player_movement.velocity_x *= 0.90;
    }
  }

  // Jump
  if (input_handler_.isSpace() && player_movement.is_grounded) {
    player_movement.acceleration_y = player_movement.jump_force;
  } else if (player_movement.acceleration_y > 0) {
    player_movement.acceleration_y = 0;
  }
}

}  // namespace platformer2d
