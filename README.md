# Platformer 2D

This is a work in progress. Having fun doing some hobby game programming in C++ with Raylib as a graphics library.

## Features

I have written my own 'engine' using ECS (Entity Component System) architecture to create:

- Physics system with gravity, acceleration, velocity, and position
- Basic collision detection using AABB
- Texture and tilemap rendering
- Input handling
- Animation system
- Tilemap editor / level design tool (in progress)

This is a toy project to learn about game programming patterns and design principles, have fun brushing up on modern C++ best practices and approaches, and gain familiarity with the C++20 standard.

## Getting Started

1. Clone this repository
2. Install dependencies (see below)
3. Build the project
4. Run the game

## Dependencies

### Raylib

The project depends on [Raylib](https://www.raylib.com/), which you can install via Homebrew or build from source (see the [Raylib documentation](https://www.raylib.com/)).

For macOS, I’ve pre-built the necessary assets and included them in the `lib/` folder. If you’re using a different OS, you’ll need to build or install Raylib and swap out the library files as appropriate.

I use a `Makefile` to build the project, so you should have `make` installed.

### nlohmann/json

This project uses [nlohmann/json](https://github.com/nlohmann/json) for JSON parsing. The library is included as a single header file (`json.hpp`) in the `lib/` folder, so you don't need to install it separately. It should work on all platforms without any additional setup.

### Optional: Bear for IDE Support

If you're using an IDE or editor that benefits from `compile_commands.json` (e.g., for code navigation or autocompletion), you can use [Bear](https://github.com/rizsotto/Bear) to generate it. Bear is **optional** and not required to build or run the project.

To install Bear on macOS:

```bash
brew install bear
```

## Build

You can build the project just running

```bash
make
```

If you want to use `bear` to generate a compile_commamnds.json

```bash
bear -- make
```

Now you can run the game with:

    bin/game

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
- lib/ is for 3rd party libs and currently contains the raylib files and nlohmann/json header
- assets/ contains art assets
