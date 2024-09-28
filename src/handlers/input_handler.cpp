#include "handlers/input_handler.h"

#include "raylib.h"

namespace platformer2d {

void InputHandler::getInput() {
  is_right_ = IsKeyDown(KEY_RIGHT);
  is_left_ = IsKeyDown(KEY_LEFT);
  is_space_ = IsKeyDown(KEY_SPACE);
}

}  // namespace platformer2d
