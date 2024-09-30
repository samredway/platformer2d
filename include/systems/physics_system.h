#pragma once

#include <unordered_map>

#include "components/collision_component.h"
#include "components/movement_component.h"
#include "components/position_component.h"

namespace platformer2d {

class PhysicsSystem {
 public:
  PhysicsSystem(
      std::unordered_map<std::string, MovementComponent>& movement_components,
      std::unordered_map<std::string, PositionComponent>& position_components,
      std::unordered_map<std::string, CollisionComponent>&
          collision_components);
  void update();

 private:
  std::unordered_map<std::string, MovementComponent>& movement_components_;
  std::unordered_map<std::string, PositionComponent>& position_components_;
  std::unordered_map<std::string, CollisionComponent>& collision_components_;
};

}  // namespace platformer2d
