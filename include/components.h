#pragma once

#include <string>
#include <unordered_map>

#include "macros.h"

namespace platformer2d {

// Helper function to retrieve a component from component map
// Always use this to get components because we get an error
// message if some reason the component is not found
template <typename Component>
Component& getComponentOrPanic(
    std::unordered_map<std::string, Component>& components,
    const std::string& tag) {
  try {
    return components.at(tag);
  } catch (const std::out_of_range& e) {
    PANIC("Component with tag '" + tag + "' not found: " + e.what());
  }
}

struct CollisionComponent {
  float width;
  float height;
  float offset_x = 0;
  float offset_y = 0;
};

struct PositionComponent {
  float x;
  float y;
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
  bool is_facing_right{true};
};

struct RenderComponent {
  std::string texture_name;
};

enum class AnimationState {
  kIdle,
  kRunning,
};

struct AnimationComponent {
  std::unordered_map<AnimationState, std::string> state_to_texture_name_map;
  std::unordered_map<AnimationState, int8_t> state_to_num_frames_map;
  AnimationState current_state = AnimationState::kIdle;
  std::unordered_map<AnimationState, float> animation_fps;
  int8_t current_frame{0};
  float scale{1.3f};
};

}  // namespace platformer2d
