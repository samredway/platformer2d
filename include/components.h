#pragma once

#include "raylib.h"

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
  float walk_force{6000};
  float mass{10.0};
  float friction_coefficient{10};
  float drag{0.2};
};

struct RenderComponent {
  Color color;
};

}  // namespace platformer2d
