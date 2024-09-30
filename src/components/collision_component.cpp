#include "components/collision_component.h"

#include <string>

#include "components/position_component.h"
#include "raylib.h"

namespace platformer2d {

CollisionComponent::CollisionComponent(const std::string& tag, float width,
                                       float height, float offset_x,
                                       float offset_y)
    : Component{tag},
      width{width},
      height{height},
      offset_x{offset_x},
      offset_y{offset_y} {}

Rectangle CollisionComponent::getCollisionBox(
    const PositionComponent& position) const {
  return Rectangle{position.x + offset_x, position.y + offset_y, width, height};
}

}  // namespace platformer2d
