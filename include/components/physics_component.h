#pragma once

namespace platformer2d {

class PhysicsComponent {
 public:
  PhysicsComponent(float position_x, float position_y, int width, int height,
                   float mass = 1.0);

  // Apply force to the physical body in X direction where a = F / m
  void applyForceX(float force);

  // Apply force to the physical body in Y direction where a = F / m
  void applyForceY(float force);

  float getPositionX() const { return position_x_; }

  float getPositionY() const { return position_y_; }

  float getVelocityX() const { return velocity_x_; }

  float getVelocityY() const { return velocity_y_; }

  int getWidth() const { return width_; }

  int getHeight() const { return height_; }

 private:
  float position_x_, position_y_;
  float velocity_x_, velocity_y_;
  float acceleration_x_, acceleration_y_;
  // Const physical characteristics
  const int width_, height_;
  const float mass_;
};

}  // namespace platformer2d
