#pragma once

#include "component.h"

namespace platformer2d {

class PhysicsComponent : public Component {
 public:
  PhysicsComponent(float position_x, float position_y, float mass = 1.0);

  // Apply force to the physical body in X direction where a = F / m
  void applyForceX(float force);

  // Apply force to the physical body in Y direction where a = F / m
  void applyForceY(float force);

  float getPositionX() const { return position_x_; }

  float getPositionY() const { return position_y_; }

  float getVelocityX() const { return velocity_x_; }

  float getVelocityY() const { return velocity_y_; }

 private:
  float position_x_, position_y_;
  float velocity_x_, velocity_y_;
  float acceleration_x_, acceleration_y_;
  float mass_;
};

}  // namespace platformer2d
