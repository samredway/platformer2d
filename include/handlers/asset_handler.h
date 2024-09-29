#pragma once

#include <string>
#include <unordered_map>

#include "raylib.h"

namespace platformer2d {

class AssetManager {
 public:
  ~AssetManager();
  void loadTexture(const std::string& name, const std::string& filename);
  void loadTexture(const std::string& name, const std::string& filename,
                   int width, int height);
  const Texture2D& getTexture(const std::string& name) const;

 private:
  std::unordered_map<std::string, Texture2D> textures_;
};

}  // namespace platformer2d
