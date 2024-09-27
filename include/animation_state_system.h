#pragma once

#include <string>
#include <unordered_map>

#include "components.h"

namespace platformer2d {

class AnimationStateSystem {
 public:
  AnimationStateSystem(
      std::unordered_map<std::string, AnimationComponent>& animations,
      std::unordered_map<std::string, MovementComponent>& movements);

  void update();

 private:
  std::unordered_map<std::string, AnimationComponent>& animations_;
  std::unordered_map<std::string, MovementComponent>& movements_;
};

}  // namespace platformer2d