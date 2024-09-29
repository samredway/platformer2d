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
    DrawTexture(texture, position.x, position.y, WHITE);
  }
}

}  // namespace platformer2d
