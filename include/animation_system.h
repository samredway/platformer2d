#pragma once

#include <unordered_map>

#include "asset_manager.h"
#include "components.h"

namespace platformer2d {

class AnimationSystem {
 public:
  AnimationSystem(
      std::unordered_map<std::string, AnimationComponent>& animations,
      std::unordered_map<std::string, PositionComponent>& positions,
      AssetManager& assets);
  void update();
  void draw() const;

 private:
  std::unordered_map<std::string, AnimationComponent>& animations_;
  std::unordered_map<std::string, PositionComponent>& positions_;
  AssetManager& assets_;
};

}  // namespace platformer2d