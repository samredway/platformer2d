#pragma once

#include "component.h"

namespace platformer2d {

struct RenderComponent : Component {
  RenderComponent(const std::string& tag, const std::string& texture_name);
  std::string texture_name;
};

}  // namespace platformer2d