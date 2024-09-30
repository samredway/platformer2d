#pragma once

#include "component.h"
#include "position_component.h"
#include "raylib.h"

namespace platformer2d {

// Collision component is used to define the collision box of an entity
// The collision box is:
// position.x + offset_x -> position.x + offset_x + width
// position.y + offset_y -> position.y + offset_y + height
struct CollisionComponent : Component {
  CollisionComponent(const std::string& tag, float width, float height,
                     float offset_x = 0, float offset_y = 0);
  float width;     // Sprite width effectively (width of collision box)
  float height;    // Sprite height effectively (height of collision box)
  float offset_x;  // Offset from the sprite's x position
  float offset_y;  // Offset from the sprite's y position

  Rectangle getCollisionBox(const PositionComponent& position) const;
};

}  // namespace platformer2d
