#include "components/render_component.h"
#include "raylib.h"

namespace platformer2d {

RenderComponent::RenderComponent(Color color) : color_(color) {
}

void RenderComponent::drawRect(float x, float y, int width, int height) const {
  DrawRectangle(x, y, width, height, color_);
}

}  // namespace platformer2d
