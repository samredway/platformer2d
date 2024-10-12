#pragma once

#include <string>

#include "json.hpp"

namespace platformer2d {

struct Tile {
  int x;
  int y;
  std::string texture_name;

  nlohmann::json toJson() const {
    nlohmann::json json;
    json["x"] = x;
    json["y"] = y;
    json["texture_name"] = texture_name;
    return json;
  }
};

}  // namespace platformer2d
