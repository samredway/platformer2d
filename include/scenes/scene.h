#pragma once

#include "managers/asset_manager.h"
#include "managers/input_manager.h"
#include "raylib.h"

namespace platformer2d {

class Scene {
 public:
  Scene(std::string name, Color color, AssetManager& asset_manager,
        InputManager& input_manager)
      : name_(name),
        background_color_(color),
        asset_manager_(asset_manager),
        input_manager_(input_manager) {}

  virtual ~Scene() = default;
  virtual void update() = 0;
  virtual void draw() const = 0;
  virtual void init() = 0;

  // Getter for name
  std::string name() const { return name_; }

 protected:
  std::string name_;
  Color background_color_;
  AssetManager& asset_manager_;
  InputManager& input_manager_;

  virtual void handleInput() = 0;  // Moved from private to protected
};

}  // namespace platformer2d
