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
  bool isE() const;
#endif

 private:
  bool is_space_;
  bool is_left_;
  bool is_right_;

#ifndef NDEBUG
  bool is_e_;
#endif
};

}  // namespace platformer2d
