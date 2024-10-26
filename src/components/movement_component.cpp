#include "components/movement_component.h"

#include <string>

namespace platformer2d {

MovementComponent::MovementComponent(const std::string& tag, float velocity_x,
                                     float velocity_y, float acceleration_x,
                                     float acceleration_y, float jump_force,
                                     float walk_force, float mass,
                                     float friction_coefficient, float drag,
                                     float air_movement_divisor,
                                     bool is_grounded, bool is_facing_right)
    : Component(tag),
      velocity_x(velocity_x),
      velocity_y(velocity_y),
      acceleration_x(acceleration_x),
      acceleration_y(acceleration_y),
      jump_force(jump_force),
      walk_force(walk_force),
      mass(mass),
      friction_coefficient(friction_coefficient),
      drag(drag),
      air_movement_divisor(air_movement_divisor),
      is_grounded(is_grounded),
      is_facing_right(is_facing_right) {}
}  // namespace platformer2d
