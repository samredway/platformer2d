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
      is_s_pressed_(false),
      is_mouse_clicked_(false)
#endif
{
}

void InputManager::getInput() {
  is_right_ = IsKeyDown(KEY_RIGHT);
  is_left_ = IsKeyDown(KEY_LEFT);
  is_space_ = IsKeyPressed(KEY_SPACE);

// Level Editor stuff DEBUG build only
#ifndef NDEBUG
  is_e_pressed_ = IsKeyPressed(KEY_E);
  is_s_pressed_ = IsKeyPressed(KEY_S);

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

// Level Editor stuff DEBUG build only
#ifndef NDEBUG
bool InputManager::isEPressed() const { return is_e_pressed_; }

bool InputManager::isSPressed() const { return is_s_pressed_; }

bool InputManager::mouseClicked() const { return is_mouse_clicked_; }

int InputManager::getMousePositionX() const { return mouse_position_x_; }

int InputManager::getMousePositionY() const { return mouse_position_y_; }
#endif

}  // namespace platformer2d
