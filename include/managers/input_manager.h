#pragma once

#include "managers/manager.h"

namespace platformer2d {

class InputManager : public Manager {
 public:
  InputManager();
  void getInput();
  bool isRight() const;
  bool isLeft() const;
  bool isSpace() const;

#ifndef NDEBUG
  bool isEPressed() const;
  bool mouseClicked() const;
  int getMousePositionX() const;
  int getMousePositionY() const;
#endif

 private:
  bool is_space_;
  bool is_left_;
  bool is_right_;

#ifndef NDEBUG
  bool is_e_pressed_;
  bool is_mouse_clicked_;
  int mouse_position_x_;
  int mouse_position_y_;
#endif
};

}  // namespace platformer2d
