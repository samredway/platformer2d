#include "game.h"

#include "constants.h"
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

// just declare a static array of tuple which has all Tiles in (name, file_name)
// pairs maybe later will dynamicall load textures based on paths like
// assets/tiles and assets/sprites. Doing it this way for now until I have a
// better idea what is best
constexpr int num_textures{18};
using TextureNameFile =
    const std::array<std::tuple<std::string, std::string>, num_textures>;
TextureNameFile texture_name_to_file{
    std::tuple{"tile_winter_ground_0", "assets/winter_ground/ground0.png"},
    std::tuple{"tile_winter_ground_1", "assets/winter_ground/ground1.png"},
    std::tuple{"tile_winter_ground_2", "assets/winter_ground/ground2.png"},
    std::tuple{"tile_winter_ground_3", "assets/winter_ground/ground3.png"},
    std::tuple{"tile_winter_ground_4", "assets/winter_ground/ground4.png"},
    std::tuple{"tile_winter_ground_5", "assets/winter_ground/ground5.png"},
    std::tuple{"tile_winter_ground_6", "assets/winter_ground/ground6.png"},
    std::tuple{"tile_winter_ground_7", "assets/winter_ground/ground7.png"},
    std::tuple{"tile_winter_ground_8", "assets/winter_ground/ground8.png"},
    std::tuple{"tile_winter_ground_9", "assets/winter_ground/ground9.png"},
    std::tuple{"tile_winter_groundIce1", "assets/winter_ground/groundIce1.png"},
    std::tuple{"tile_winter_groundIce2", "assets/winter_ground/groundIce2.png"},
    std::tuple{"tile_winter_groundIce3", "assets/winter_ground/groundIce3.png"},
    std::tuple{"tile_winter_groundl", "assets/winter_ground/groundl.png"},
    std::tuple{"tile_winter_groundr", "assets/winter_ground/groundr.png"},
    std::tuple{"tile_winter_ice", "assets/winter_ground/ice.png"},
};

Game::Game() : input_manager_(), asset_manager_() {
  // Setup Window
  initWindow();

  // Load all textures
  for (auto& pair : texture_name_to_file) {
    asset_manager_.loadTexture(std::get<0>(pair), std::get<1>(pair), kTileSize,
                               kTileSize);
  }

  // Load in sprites (different size than tiles)
  asset_manager_.loadTexture("pink_monster_idle",
                             "assets/Pink_Monster_Idle_4.png");
  asset_manager_.loadTexture("pink_monster_run",
                             "assets/Pink_Monster_Run_6.png");

  current_scene_ = std::make_unique<LevelScene>(asset_manager_, input_manager_);
  current_scene_->init();
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
      setCurrentScene(
          std::make_unique<LevelEditor>(asset_manager_, input_manager_));
      // Add width of tile picker to screen width
      resizeWindow(kScreenWidth + kTilePickerWidth, kScreenHeight);
    } else {
      setCurrentScene(
          std::make_unique<LevelScene>(asset_manager_, input_manager_));
      resizeWindow(kScreenWidth, kScreenHeight);
    }
    current_scene_->init();
  }
#endif
}

void Game::setCurrentScene(std::unique_ptr<Scene> new_scene) {
  current_scene_ = std::move(new_scene);
}

void initWindow() {
  InitWindow(kScreenWidth, kScreenHeight, "2D Platform Game");
  SetTargetFPS(kTargetFPS);
}

void resizeWindow(const int width, const int height) {
  SetWindowSize(width, height);
}

}  // namespace platformer2d
