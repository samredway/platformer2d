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
      is_e_pressed_(false),
      is_mouse_clicked_(false)
#endif
{
}

void InputManager::getInput() {
  is_right_ = IsKeyDown(KEY_RIGHT);
  is_left_ = IsKeyDown(KEY_LEFT);
  is_space_ = IsKeyDown(KEY_SPACE);

#ifndef NDEBUG
  // Level Editor stuff DEBUG build only
  is_e_pressed_ = IsKeyPressed(KEY_E) ? true : false;

  if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
    is_mouse_clicked_ = true;
    mouse_position_x_ = GetMouseX();
    mouse_position_y_ = GetMouseY();
  } else {
    is_mouse_clicked_ = false;
  }
#endif
}

bool InputManager::isRight() const { return is_right_; }

bool InputManager::isLeft() const { return is_left_; }

bool InputManager::isSpace() const { return is_space_; }

#ifndef NDEBUG
bool InputManager::isEPressed() const { return is_e_pressed_; }

bool InputManager::mouseClicked() const {
  DLOG("Get mouse clicked = " << (is_mouse_clicked_ ? "true" : "false"));
  return is_mouse_clicked_;
}

int InputManager::getMousePositionX() const { return mouse_position_x_; }

int InputManager::getMousePositionY() const { return mouse_position_y_; }
#endif

}  // namespace platformer2d
