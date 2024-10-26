# Platformer 2D

This is a work in progress. Having fun doing some hobby game programming in C++ with Raylib as a graphics library.

As someone that works primarily in Python at work, this is a toy project with the aim of having fun learning about game programming patterns and design principles, while brushing up on modern C++ best practices and approaches, and gain familiarity with the C++20 standard.

One thing I have learnt doing this is that developing a game engine is HARD. Even for a simple platformer there is a lot of domain specific knowledge and design patterns - so there is definitely a lot of room for improvement here!

## Features

I have written my own 'engine' using ECS (Entity Component System) architecture to create:

- Physics system with gravity, acceleration, velocity, and position
- Basic collision detection using AABB overlap checks and resolution with minimum translation vectors
- Texture and tilemap rendering
- Input handling
- Animation system
- Tilemap editor / level design tool

## Getting Started

1. Clone this repository
2. Install dependencies (see below)
3. Build the project
4. Run the game

## Dependencies

### CMake and Ninja

This project uses CMake as its build system and Ninja as the build tool. You'll need to have both installed on your system. On macOS, you can install them via Homebrew:

```bash
brew install cmake ninja
```

For other platforms, please refer to the official installation guides for [CMake](https://cmake.org/install/) and [Ninja](https://ninja-build.org/).

### Raylib and nlohmann/json

The project depends on [Raylib](https://www.raylib.com/) and [nlohmann/json](https://github.com/nlohmann/json). These dependencies are automatically managed by CMake and will be downloaded and built during the project configuration.

## Build

This project uses a cross-platform CMake build system with Ninja, but it has only been tested on macOS. To build the project:

1. Configure the project:
   ```bash
   cmake --preset default
   ```

2. Build the project:
   ```bash
   cmake --build --preset default
   ```

The executable will be created in the `build/bin` directory.

## Run

After building, you can run the game from the build directory:

```bash
./build/bin/Platformer2d
```

## Style

I try to follow the [google style guide](https://google.github.io/styleguide/cppguide.html) pretty much to the letter.

Some key points regarding keeping code simple and making memory management safer are:

- No Exceptions: return error codes instead of throwing exceptions
- Avoid New and Delete
  - I prefer stack to heap
  - Use std containers like Vector to manage allocation automatically
  - If memory must be heap allocated then use smart pointers over raw pointers wherever possible
- RAII (Resource Acquisition Is Initialization) initialise resources in constructor and teardown in destructor
- Avoid deep inheritance (1-2 levels is a max and should generally even then be avoided to use composition and or interfaces)

## Structure

The main folders are as follows:

- include/ contains the .h files for the project
- src/ contains the cpp files
- assets/ contains art assets
