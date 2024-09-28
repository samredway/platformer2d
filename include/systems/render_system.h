#pragma once

#include <string>
#include <unordered_map>

#include "components.h"
#include "handlers/asset_handler.h"

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
