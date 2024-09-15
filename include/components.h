#pragma once

#include "raylib.h"

namespace platformer2d {

struct PhysicsComponent {
  float position_x;
  float position_y;
  int width;
  int height;
  float velocity_x{0};
  float velocity_y{0};
  float acceleration_x{0};
  float acceleration_y{0};
  float walk_force{10.0};
  float mass{1.0};
};

struct RenderComponent {
  Color color;
};

}  // namespace platformer2d
