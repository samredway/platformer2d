#include "animation_state_system.h"

namespace platformer2d {

AnimationStateSystem::AnimationStateSystem(
    std::unordered_map<std::string, AnimationComponent>& animations,
    std::unordered_map<std::string, MovementComponent>& movements)
    : animations_(animations), movements_(movements) {
}

void AnimationStateSystem::update() {
  // TODO: Remove this and think of a better way to handle this
  // This works for player but not for enemies. Will likely need
  // to have a field `type` for the different types of characters and
  // then have a different set of rules for each type of character
  MovementComponent& movement = movements_.at("player");
  if (movement.velocity_x != 0 && movement.is_grounded) {
    animations_.at("player").current_state = AnimationState::kRunning;
  } else {
    animations_.at("player").current_state = AnimationState::kIdle;
  }
}

}  // namespace platformer2d