#pragma once

#include <string>
#include <unordered_map>

#include "macros.h"

namespace platformer2d {

// Helper function to retrieve a component from component map
// Always use this to get components because we get an error
// message if some reason the component is not found
template <typename ComponentT>
ComponentT& getComponentOrPanic(
    std::unordered_map<std::string, ComponentT>& components,
    const std::string& tag) {
  try {
    return components.at(tag);
  } catch (const std::out_of_range& e) {
    PANIC("Component with tag '" + tag + "' not found: " + e.what());
  }
}

struct Component {
  Component(std::string tag) : entity_tag(tag) {}

  std::string entity_tag;
};

}  // namespace platformer2d
