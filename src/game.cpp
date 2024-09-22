#include <string>

#include "components.h"
#include "constants.h"
#include "game.h"
#include "input_handler.h"
#include "macros.h"
#include "physics_system.h"
#include "raylib.h"

namespace platformer2d {

// Game Entities
const std::string playerTag{"player"};

Game::Game(int width, int height)
    : screen_width_(width),
      screen_height_(height),
      input_handler_(),
      physics_(movement_components_, position_components_,
               collision_components_) {
  // Setup Window
  InitWindow(screen_width_, screen_height_, "2D Platform Game");
  SetTargetFPS(kTargetFPS);

  // Initialise entities //////////////////////////////////////////////////////
  position_components_.emplace(
      playerTag,
      PositionComponent{(float)width / 2, (float)height / 2, 40, 40});
  movement_components_.emplace(playerTag, MovementComponent{});
  render_components_.emplace(playerTag, RenderComponent{RED});
  collision_components_.emplace(playerTag, CollisionComponent{});

  // Floor tiles
  for (int i = 0; i < 8; i++) {
    const std::string tileTag{std::format("tile{}", i)};
    const float x{(width / 2.0f) - ((4.0f - i) * 40)};
    position_components_.emplace(
        tileTag, PositionComponent{x, (float)height - 40, 40, 40});
    render_components_.emplace(tileTag, RenderComponent{BLACK});
    collision_components_.emplace(tileTag, CollisionComponent{});
  }

  // Central tile to do x collision on
  const std::string tileTag{std::format("tile8")};
  const float x{(width / 2.0f) - 20};
  position_components_.emplace(
      tileTag, PositionComponent{x, (float)height - 80, 40, 40});
  render_components_.emplace(tileTag, RenderComponent{BLACK});
  collision_components_.emplace(tileTag, CollisionComponent{});

  /////////////////////////////////////////////////////////////////////////////
}

Game::~Game() {
  CloseWindow();
}

void Game::update() {
  handleInput();
  physics_.update();
}

void Game::draw() const {
  BeginDrawing();
  ClearBackground(SKYBLUE);
  processRendering();
  EndDrawing();
}

void Game::handleInput() {
  // Would crash if player not defined somehow
  auto& player_movement{movement_components_.at(playerTag)};

  input_handler_.getInput();

  const float rate_accleration{player_movement.walk_force /
                               player_movement.mass};

  // Accelerate in direction pressed.
  // If nothing pressed then decellerate unless at rest then stop
  if (input_handler_.isRight() && player_movement.is_grounded) {
    // Accelerate right
    player_movement.acceleration_x = rate_accleration;
  } else if (input_handler_.isLeft() && player_movement.is_grounded) {
    // Accelerate left
    player_movement.acceleration_x = -rate_accleration;
  } else {
    player_movement.acceleration_x = 0;
    // Artbitrary decelleration rate to mimic players own force in
    // slowing down
    if (player_movement.is_grounded) {
      player_movement.velocity_x *= 0.90;
    }
  }

  // Jump
  if (input_handler_.isSpace() && player_movement.is_grounded) {
    // Arbitrary jump force
    player_movement.acceleration_y = 6000;
  } else if (player_movement.acceleration_y > 0) {
    player_movement.acceleration_y = 0;
  }
}

// Processors for component collections
// ///////////////////////////////////////
void Game::processRendering() const {
  for (const auto& render_pair : render_components_) {
    const std::string entity_tag{render_pair.first};
    const PositionComponent& position{position_components_.at(entity_tag)};
    const Color color{render_pair.second.color};
    DrawRectangle(position.x, position.y, position.width, position.height,
                  color);
  }
}

////////////////////////////////////////////////////////////////////////////////
}  // namespace platformer2d
