#include "managers/asset_manager.h"

#include "debug.h"

namespace platformer2d {

AssetManager::AssetManager() {}

// load texture at its actual size
void AssetManager::loadTexture(const std::string& name,
                               const std::string& filename) {
  // Dont overwrite existing textures
  if (textures_.contains(name)) return;
  Texture2D texture = LoadTexture(filename.c_str());
  textures_[name] = texture;
}

// Load texture and set its size
void AssetManager::loadTexture(const std::string& name,
                               const std::string& filename, int width,
                               int height) {
  if (textures_.contains(name)) return;
  Texture2D texture = LoadTexture(filename.c_str());
  texture.width = width;
  texture.height = height;
  textures_[name] = texture;
}

const Texture2D& AssetManager::getTexture(const std::string& name) const {
  if (!textures_.contains(name)) {
    PANIC("texuture " << name
                      << " not in AssetManager.textures_ : maybe a misnamed "
                         "file or texture name?");
  }
  return textures_.at(name);
}

AssetManager::~AssetManager() {
  for (auto& pair : textures_) {
    UnloadTexture(pair.second);
  }
}

}  // namespace platformer2d
