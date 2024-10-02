#pragma once

namespace platformer2d {

// Base class for all managers which are essentially singletons
// And should never be copied or moved.
class Manager {
 public:
  Manager() = default;
  virtual ~Manager() = default;

  // Delete copy and move constructors
  Manager(Manager&&) = delete;
  Manager& operator=(Manager&&) = delete;
  Manager(const Manager&) = delete;
  Manager& operator=(const Manager&) = delete;
};

}  // namespace platformer2d
