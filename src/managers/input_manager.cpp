#include "macros.h"
#include "managers/input_manager.h"
#include "raylib.h"

namespace platformer2d {

InputManager::InputManager()
    : is_space_(false),
      is_left_(false),
      is_right_(false)
#ifndef NDEBUG
      ,
      is_e_(false)
#endif
{
}

void InputManager::getInput() {
  is_right_ = IsKeyDown(KEY_RIGHT);
  is_left_ = IsKeyDown(KEY_LEFT);
  is_space_ = IsKeyDown(KEY_SPACE);

#ifndef NDEBUG
  is_e_ = IsKeyPressed(KEY_E) ? true : false;
#endif
}

bool InputManager::isRight() const { return is_right_; }

bool InputManager::isLeft() const { return is_left_; }

bool InputManager::isSpace() const { return is_space_; }

#ifndef NDEBUG
bool InputManager::isE() const { return is_e_; }
#endif

}  // namespace platformer2d
