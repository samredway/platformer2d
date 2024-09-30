#include "components/animation_component.h"

#include <string>
#include <unordered_map>

#include "macros.h"

namespace platformer2d {

AnimationComponent::AnimationComponent(const std::string& tag,
                                       const float scale)
    : Component(tag),
      scale(scale),
      state_to_texture_name_map(),
      state_to_animation_fps(),
      state_to_num_frames_map() {}

std::string toString(AnimationState state) {
  switch (state) {
    case AnimationState::kIdle:
      return "Idle";
    case AnimationState::kRunning:
      return "Running";
    // Add other cases as needed
    default:
      return "Unknown";
  }
}

int8_t AnimationComponent::getCurrentNumFrames() const {
  try {
    return state_to_num_frames_map.at(current_state);
  } catch (const std::out_of_range& e) {
    PANIC("No frames for entity "
          << entity_tag << " current state: " + toString(current_state));
  }
}

float AnimationComponent::getCurrentAnimationFPS() const {
  try {
    return state_to_animation_fps.at(current_state);
  } catch (const std::out_of_range& e) {
    PANIC("No FPS for entity " << entity_tag
                               << " current state: " + toString(current_state));
  }
}

std::string AnimationComponent::getCurrentTextureName() const {
  try {
    return state_to_texture_name_map.at(current_state);
  } catch (const std::out_of_range& e) {
    PANIC("No texture for entity "
          << entity_tag << " current state: " + toString(current_state));
  }
}

void AnimationComponent::setStateToTextureName(
    AnimationState state, const std::string& texture_name) {
  state_to_texture_name_map[state] = texture_name;
}

void AnimationComponent::setStateToAnimationFPS(AnimationState state,
                                                float animation_fps) {
  state_to_animation_fps[state] = animation_fps;
}

void AnimationComponent::setStateToNumFrames(AnimationState state,
                                             int8_t num_frames) {
  state_to_num_frames_map[state] = num_frames;
}

}  // namespace platformer2d