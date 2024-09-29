#pragma once

#include <unordered_map>

#include "components.h"
#include "handlers/asset_handler.h"

namespace platformer2d {

class AnimationSystem {
 public:
  AnimationSystem(
      std::unordered_map<std::string, AnimationComponent>& animations,
      std::unordered_map<std::string, PositionComponent>& positions,
      std::unordered_map<std::string, MovementComponent>& movements,
      AssetManager& assets);
  void update();
  void draw() const;

 private:
  std::unordered_map<std::string, AnimationComponent>& animations_;
  std::unordered_map<std::string, PositionComponent>& positions_;
  std::unordered_map<std::string, MovementComponent>& movements_;
  AssetManager& assets_;
  int frame_number_;
};

}  // namespace platformer2d