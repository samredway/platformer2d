# Raylib Game Template

A quick setup template for games using C++ Raylib 5.0

The aim of this repo is just to provide some handy boilerplate and the include files to get going quickly.

I use this setup on my MacBook Pro. Have not tried on Linux or Windows

## Dependencies

I use `bear` and `make` as a build tool. Installable on Mac as `brew install bear` see build below for usage.

The project depends on:

- Raylib: this can be brew installed, or build from scratch see the [docs](https://www.raylib.com/), but I have build the assets separately and included in the lib folder, which are for MacOs.

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

- No Exceptions (hard to guaranty memory safety when the stack is deleted)
- Avoid New and Delete
  - I prefer stack to heap
  - Use std containers like Vector to manage allocation automatically
  - If memory must be heap allocated then use smart pointers over raw pointers wherever possible
- RAII (Resource Acquisition Is Initialization) initialise resources in constructor and teardown in destructor
- Avoid deep inheritance (1-2 levels is a max and should generally even then be avoided to use composition and or interfaces)

## Structure

The main folders are as follows:

- include contains the .h files for your own src code
- src contains the cpp files
- lib is for 3rd party libs and currently contains the raylib files

