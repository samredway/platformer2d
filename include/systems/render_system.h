#pragma once

#include <string>
#include <unordered_map>

#include "components/position_component.h"
#include "components/render_component.h"
#include "managers/asset_manager.h"

namespace platformer2d {

class RenderSystem {
 public:
  RenderSystem(
      std::unordered_map<std::string, PositionComponent>& position_components,
      std::unordered_map<std::string, RenderComponent>& render_components,
      AssetManager& assets);
  void draw() const;

 private:
  std::unordered_map<std::string, PositionComponent>& position_components_;
  std::unordered_map<std::string, RenderComponent>& render_components_;
  AssetManager& assets_;
};

}  // namespace platformer2d
