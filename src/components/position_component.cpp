#include "components/position_component.h"

#include <string>

namespace platformer2d {

PositionComponent::PositionComponent(const std::string& tag, float x, float y)
    : Component{tag}, x{x}, y{y} {}

}  // namespace platformer2d
