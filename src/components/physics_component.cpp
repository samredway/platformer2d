#include "components/physics_component.h"

namespace platformer2d {

PhysicsComponent::PhysicsComponent(float x, float y, int width, int height,
                                   float mass)
    : position_x_(x),
      position_y_(y),
      velocity_x_(0),
      velocity_y_(0),
      acceleration_x_(0),
      acceleration_y_(0),
      width_(width),
      height_(height),
      mass_(mass) {
}

void PhysicsComponent::applyForceX(float force) {
  acceleration_x_ += force / mass_;
}

void PhysicsComponent::applyForceY(float force) {
  acceleration_y_ += force / mass_;
}

}  // namespace platformer2d
