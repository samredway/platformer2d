#include "game.h"

#include "constants.h"
#include "macros.h"
#include "raylib.h"
#include "scenes/level_scene.h"

// Level editor should only be in non Release builds keep the binary
// smaller buy not compiling it in
#ifndef NDEBUG
#include "scenes/level_editor.h"
#endif

namespace platformer2d {

// Forward declare free helper function
void initWindow();
void resizeWindow(const int width, const int height);

Game::Game() : input_manager_(), asset_manager_(), scenes_() {
  // Setup Window
  initWindow();

  // Currently we load the asset manager with all textures in a lazy fashion
  // as they are needed. However all textures are held in RAM all the time
  // if this becomes a problem we may need to let each scene load and destruct
  // its own textures so as only to hold the textures in RAM that are required
  // in the given scene
  scenes_["level"] =
      std::make_unique<LevelScene>(asset_manager_, input_manager_);
  scenes_["level"]->init();
  current_scene_ = scenes_["level"].get();

#ifndef NDEBUG
  scenes_["editor"] =
      std::make_unique<LevelEditor>(asset_manager_, input_manager_);
  scenes_["editor"]->init();
#endif
}

Game::~Game() { CloseWindow(); }

void Game::update() {
  handleInput();
  current_scene_->update();
}

void Game::draw() const {
  BeginDrawing();
  current_scene_->draw();
  EndDrawing();
}

void Game::handleInput() {
  input_manager_.getInput();
#ifndef NDEBUG
  // Toggle editor mode
  if (input_manager_.isEPressed()) {
    if (current_scene_->name() == "level") {
      setCurrentScene("editor");
      // Add width of tile picker to screen width
      resizeWindow(kScreenWidth + kTilePickerWidth, kScreenHeight);
    } else {
      setCurrentScene("level");
      resizeWindow(kScreenWidth, kScreenHeight);
    }
  }
#endif
}

void Game::setCurrentScene(const std::string& scene_name) {
  auto it = scenes_.find(scene_name);
  if (it != scenes_.end()) {
    current_scene_ = it->second.get();
  } else {
    PANIC("Scene not found");
  }
}

void initWindow() {
  InitWindow(kScreenWidth, kScreenHeight, "2D Platform Game");
  SetTargetFPS(kTargetFPS);
}

void resizeWindow(const int width, const int height) {
  SetWindowSize(width, height);
}

}  // namespace platformer2d
