#pragma once

namespace platformer2d {

class InputHandler {
 public:
  void getInput();

  bool isDirectionKeyPressed() const {
    return is_up_ || is_down_ || is_right_ || is_left_;
  }

  bool isRight() const { return is_right_; }

  bool isLeft() const { return is_left_; }

  bool isUp() const { return is_up_; }

  bool isDown() const { return is_down_; }

 private:
  bool is_up_ = false;
  bool is_down_ = false;
  bool is_left_ = false;
  bool is_right_ = false;
};

}  // namespace platformer2d
