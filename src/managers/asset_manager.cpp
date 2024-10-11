#include "macros.h"
#include "managers/asset_manager.h"

namespace platformer2d {

AssetManager::AssetManager() {}

// load texture at its actual size
void AssetManager::loadTexture(const std::string& name,
                               const std::string& filename) {
  // Dont overwrite existing textures
  if (isTextureLoaded(name)) return;
  Texture2D texture = LoadTexture(filename.c_str());
  textures_[name] = texture;
}

// Load texture and set its size
void AssetManager::loadTexture(const std::string& name,
                               const std::string& filename, int width,
                               int height) {
  if (isTextureLoaded(name)) return;
  Texture2D texture = LoadTexture(filename.c_str());
  texture.width = width;
  texture.height = height;
  textures_[name] = texture;
}

const Texture2D& AssetManager::getTexture(const std::string& name) const {
  auto it = textures_.find(name);
  if (it == textures_.end()) {
    PANIC("texuture " << name
                      << " not in AssetManager.textures_ : maybe a misnamed "
                         "file or texture name?");
  }
  return it->second;
}

AssetManager::~AssetManager() {
  for (auto& pair : textures_) {
    UnloadTexture(pair.second);
  }
}

bool AssetManager::isTextureLoaded(std::string texture_name) const {
  auto text_it{textures_.find(texture_name)};
  if (text_it != textures_.end()) return true;
  return false;
}

}  // namespace platformer2d
