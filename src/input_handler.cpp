#include "input_handler.h"
#include "raylib.h"

namespace platformer2d {

void InputHandler::getInput() {
  is_right_ = IsKeyDown(KEY_RIGHT);
  is_left_ = IsKeyDown(KEY_LEFT);
  is_up_ = IsKeyDown(KEY_UP);
  is_down_ = IsKeyDown(KEY_DOWN);
}

}  // namespace platformer2d
