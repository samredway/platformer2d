#include "animation_system.h"
#include "constants.h"
#include "raylib.h"

namespace platformer2d {

AnimationSystem::AnimationSystem(
    std::unordered_map<std::string, AnimationComponent>& animations,
    std::unordered_map<std::string, PositionComponent>& positions,
    AssetManager& assets)
    : animations_(animations),
      positions_(positions),
      assets_(assets),
      frame_number_(0) {
}

void AnimationSystem::update() {
  frame_number_++;

  // Reset the frame number if it's too large
  if (frame_number_ > kTargetFPS * 10) {
    frame_number_ = 0;
  } 
}

void AnimationSystem::draw() const {
  for (const auto& pair : animations_) {
    auto& position{positions_.at(pair.first)};
    auto& animation{pair.second};
    std::string texture_name{
        animation.state_to_texture_name_map.at(animation.current_state)};
    Texture2D animation_frames{assets_.getTexture(texture_name)};
    int8_t num_frames{
        animation.state_to_num_frames_map.at(animation.current_state)};

    // Update the animation frame at a rate of roughly animation_fps
    int current_frame{static_cast<int>(frame_number_ /
                                       (kTargetFPS * animation.animation_fps.at(animation.current_state)))};
    current_frame %= num_frames;

    const float sprite_width = (float)animation_frames.width / num_frames;
    const float sprite_pos_x = current_frame * sprite_width;

    Rectangle frameRec = {sprite_pos_x, 0, sprite_width,
                          (float)animation_frames.height};

    // Destination rectangle (this controls the position and scaling)
    float scale = 1.3f;
    Rectangle destRec = {
        position.x,                             // Destination X position
        position.y,                             // Destination Y position
        sprite_width * scale,                   // Destination width (scaled)
        (float)animation_frames.height * scale  // Destination height (scaled)
    };

    // Origin for rotation/scaling (set to the center of the texture)
    Vector2 origin = {0.0f, 0.0f};

    // TODO Flip the sprite if moving left
    if (!position.is_facing_right) {
      frameRec.width = -sprite_width;  // Flip the sprite horizontally
    }

    // Draw the texture using DrawTexturePro, which supports scaling
    DrawTexturePro(animation_frames, frameRec, destRec, origin, 0.0f, WHITE);
  }
}

}  // namespace platformer2d
