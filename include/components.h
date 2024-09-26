#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

namespace platformer2d {

struct CollisionComponent {};

struct PositionComponent {
  float x;
  float y;
  int width;
  int height;
};

struct MovementComponent {
  float velocity_x{0};
  float velocity_y{0};
  float acceleration_x{0};
  float acceleration_y{0};
  float walk_force{300};
  float mass{10.0};
  float friction_coefficient{10};
  float drag{0.05};
  bool is_grounded{false};
};

struct RenderComponent {
  std::string texture_name;
};

struct AnimationComponent {
  std::unordered_map<std::string, std::string> state_to_texture_name_map;
  std::unordered_map<std::string, int8_t> state_to_num_frames_map;
  std::string current_state = "idle";  // eg idle, walking, running
};

}  // namespace platformer2d
