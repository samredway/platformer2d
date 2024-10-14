cmake_minimum_required(VERSION 3.28)

# Project Name and Language
project(Platformer2d LANGUAGES CXX)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable compile commands (generates compile_commands.json)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Specify Build Types
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Add Compiler Flags based on Build Type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g -O0 )
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O2 -DNDEBUG)
endif()

# Include FetchContent Module
include(FetchContent)

# Fetch and Add Raylib
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG 4.5.0
)
FetchContent_MakeAvailable(raylib)

# Fetch and Add nlohmann_json
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.10.5
)

FetchContent_MakeAvailable(json)

set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)

# Include Directories
include_directories(include)

# Gather Source Files
file(GLOB_RECURSE SRCS "src/*.cpp")

# Define the Executable
add_executable(${PROJECT_NAME} ${SRCS})

# Apply compile options specifically to your target
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror)

# Link Libraries
target_link_libraries(${PROJECT_NAME} PRIVATE raylib nlohmann_json::nlohmann_json)

# Specify Output Directories
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

# Installation Rules
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
