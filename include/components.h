#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

#include "systems/animation_state.h"

namespace platformer2d {

struct CollisionComponent {};

struct PositionComponent {
  float x;
  float y;
  int width;
  int height;
  bool is_facing_right{true};
};

struct MovementComponent {
  float velocity_x{0};
  float velocity_y{0};
  float acceleration_x{0};
  float acceleration_y{0};
  float jump_force{225};
  float walk_force{325};
  float mass{10.0};
  float friction_coefficient{10};
  float drag{0.05};
  bool is_grounded{false};
};

struct RenderComponent {
  std::string texture_name;
};

struct AnimationComponent {
  std::unordered_map<AnimationState, std::string> state_to_texture_name_map;
  std::unordered_map<AnimationState, int8_t> state_to_num_frames_map;
  AnimationState current_state = AnimationState::kIdle;
  std::unordered_map<AnimationState, float> animation_fps;
  int8_t current_frame{0};
};

}  // namespace platformer2d
