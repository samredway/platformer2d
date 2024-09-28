#pragma once

namespace platformer2d {

class InputHandler {
 public:
  void getInput();

  bool isRight() const { return is_right_; }

  bool isLeft() const { return is_left_; }

  bool isSpace() const { return is_space_; }

 private:
  bool is_space_{false};
  bool is_left_{false};
  bool is_right_{false};
};

}  // namespace platformer2d
