#pragma once

#include "raylib.h"

namespace platformer2d {

// Responsible for all rendering. This simple render component
// Only draws rect. Probably will want to add one later that can
// render sprite and animation details
class RenderComponent {
 public:
  RenderComponent(Color color);
  void drawRect(float x, float y, int width, int height) const;

 private:
  Color color_;
};

}  // namespace platformer2d
