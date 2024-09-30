#pragma once

#include "component.h"

namespace platformer2d {

struct MovementComponent : Component {
  MovementComponent(const std::string& tag, float velocity_x = 0,
                    float velocity_y = 0, float acceleration_x = 0,
                    float acceleration_y = 0, float jump_force = 225,
                    float walk_force = 325, float mass = 10.0,
                    float friction_coefficient = 10, float drag = 0.05,
                    bool is_grounded = false, bool is_facing_right = true);

  float velocity_x;
  float velocity_y;
  float acceleration_x;
  float acceleration_y;
  float jump_force;
  float walk_force;
  float mass;
  float friction_coefficient;
  float drag;
  bool is_grounded;
  bool is_facing_right;
};

}  // namespace platformer2d
