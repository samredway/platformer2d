#include "handlers/asset_handler.h"

namespace platformer2d {

void AssetManager::loadTexture(const std::string& name,
                               const std::string& filename) {
  Texture2D texture = LoadTexture(filename.c_str());
  textures_[name] = texture;
}

void AssetManager::loadTexture(const std::string& name,
                               const std::string& filename, int width,
                               int height) {
  Texture2D texture = LoadTexture(filename.c_str());
  texture.width = width;
  texture.height = height;
  textures_[name] = texture;
}

const Texture2D& AssetManager::getTexture(const std::string& name) const {
  return textures_.at(name);
}

AssetManager::~AssetManager() {
  for (auto& pair : textures_) {
    UnloadTexture(pair.second);
  }
}

}  // namespace platformer2d
