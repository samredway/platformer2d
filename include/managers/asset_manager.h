#pragma once

#include <string>
#include <unordered_map>

#include "managers/manager.h"
#include "raylib.h"

namespace platformer2d {

typedef std::unordered_map<std::string, Texture2D> TextureMap;

class AssetManager : public Manager {
 public:
  AssetManager();
  ~AssetManager();

  // Should be a singleton effectively so remving copy and move constructors
  AssetManager(const AssetManager&) = delete;
  AssetManager& operator=(const AssetManager&) = delete;
  AssetManager(AssetManager&&) = delete;
  AssetManager& operator=(AssetManager&&) = delete;

  void loadTexture(const std::string& name, const std::string& filename);
  void loadTexture(const std::string& name, const std::string& filename,
                   int width, int height);

  const TextureMap& getTextures() const { return textures_; }

  const Texture2D& getTexture(const std::string& name) const;

 private:
  bool isTextureLoaded(std::string texture_name) const;

  // private members
  std::unordered_map<std::string, Texture2D> textures_;
};

}  // namespace platformer2d
