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

I use `bear` and `make` as a build tool. Installable on Mac as `brew install bear` see build below for usage.

This approach works for me on MacOs, I have not tried it on other platforms.

The project depends on:

- Raylib: this can be brew installed, or build from scratch see the [docs](https://www.raylib.com/), but I have build the assets separately and included in the lib folder (for MacOs). If you are using a different OS you will need to build or install it yourself and swap out the lib files.

## Build

This project uses Makefile for build instructions and bear to build the compile_commands.json.

To build the compile_commmands.json just run

    bear -- make

This will build the project and update the compile_commands.json

You can run the game now with:

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

- include contains the .h files for the project
- src contains the cpp files
- lib is for 3rd party libs and currently contains the raylib files
- assets is for art assets