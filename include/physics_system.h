#pragma once

#include <functional>
#include <unordered_map>

#include "components.h"

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
  void updateVelocityX(MovementComponent& movement_component, float delta_time);
  void updateVelocityY(MovementComponent& movement_component, float delta_time);

  std::unordered_map<std::string, MovementComponent>& movement_components_;
  std::unordered_map<std::string, PositionComponent>& position_components_;
  std::unordered_map<std::string, CollisionComponent>& collision_components_;
};

}  // namespace platformer2d
