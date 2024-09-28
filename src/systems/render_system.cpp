#include "systems/render_system.h"

#include <string>

#include "raylib.h"

namespace platformer2d {

RenderSystem::RenderSystem(
    std::unordered_map<std::string, PositionComponent>& position_components,
    std::unordered_map<std::string, RenderComponent>& render_components,
    AssetManager& assets)
    : position_components_(position_components),
      render_components_(render_components),
      assets_(assets) {}

void RenderSystem::draw() const {
  for (const auto& render_pair : render_components_) {
    const std::string entity_tag{render_pair.first};
    const PositionComponent& position{position_components_.at(entity_tag)};
    const Texture2D& texture{
        assets_.getTexture(render_pair.second.texture_name)};
    constexpr float kRotation = 0.0;
    // to get scale its 1/(actual_value/required_value) I am enforcing aligment
    // by height so as blocks match for walking surface. Maybe I should just do
    // this in an art program and allow scale as a compnent field so it can be
    // tweaked at will though
    const float scale = 1 / (texture.height / (float)position.height);
    DrawTextureEx(texture, Vector2(position.x, position.y), kRotation, scale,
                  WHITE);
  }
}

}  // namespace platformer2d
