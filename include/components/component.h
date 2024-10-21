#pragma once

#include <source_location>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "debug.h"

namespace platformer2d {

// Helper function to retrieve a component from component map
// Always use this to get components because we get an error
// message if for some reason the component is not found
template <typename ComponentT>
ComponentT& getComponentOrPanic(
    std::unordered_map<std::string, ComponentT>& components,
    const std::string& tag,
    const std::source_location& location = std::source_location::current()) {
  try {
    return components.at(tag);
  } catch (const std::out_of_range& e) {
    PANIC("Component with tag '" + tag + "' not found at " +
          location.file_name() + ":" + std::to_string(location.line()) + ": " +
          e.what());
  }
}

struct Component {
  Component(std::string tag) : entity_tag(tag) {}

  std::string entity_tag;
};

}  // namespace platformer2d
