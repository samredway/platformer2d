#pragma once

#include "component.h"

namespace platformer2d {

struct PositionComponent : Component {
  PositionComponent(const std::string& tag, float x, float y);
  float x;
  float y;
};

}  // namespace platformer2d