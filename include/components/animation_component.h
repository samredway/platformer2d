#pragma once

#include "component.h"

namespace platformer2d {

enum class AnimationState {
  kIdle,
  kRunning,
};

class AnimationComponent : public Component {
 public:
  AnimationComponent(const std::string& tag, const float scale = 1.0f);

  // Getters
  std::string getCurrentTextureName() const;
  int8_t getCurrentNumFrames() const;
  float getCurrentAnimationFPS() const;

  // Setters
  void setStateToTextureName(AnimationState state,
                             const std::string& texture_name);
  void setStateToAnimationFPS(AnimationState state, float animation_fps);
  void setStateToNumFrames(AnimationState state, int8_t num_frames);

  // Public Data
  AnimationState current_state = AnimationState::kIdle;
  float scale{1.0f};
  int8_t current_frame{0};

 private:
  std::unordered_map<AnimationState, std::string> state_to_texture_name_map;
  std::unordered_map<AnimationState, float> state_to_animation_fps;
  std::unordered_map<AnimationState, int8_t> state_to_num_frames_map;
};

}  // namespace platformer2d