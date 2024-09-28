#pragma once

namespace platformer2d {

class Scene {
 public:
  virtual ~Scene() = default;
  virtual void update() = 0;
  virtual void draw() const = 0;

  // Each scene will hold all its own component collections privately
  // And will be initialised with references to relevant systems
};

}  // namespace platformer2d
