# Compiler and flags
CXX = clang++
CXXFLAGS = -std=c++20 -Iinclude -Ilib -Wall -Wextra -Werror

# Raylib library and framework dependencies
LIBS = -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL lib/libraylib.a

# Source files (excluding main.cpp files in cmd/)
SRCS = $(shell find src -type f -name "*.cpp")

# Object files
OBJS = $(patsubst src/%.cpp, build/%.o, $(SRCS))

# Targets
GAME_TARGET = bin/game
LEVEL_EDITOR_TARGET = bin/level_editor

# Default build is a debug build for both targets
all: debug

# Debug build
debug: CXXFLAGS += -g -O0
debug: $(GAME_TARGET) $(LEVEL_EDITOR_TARGET)

# Release build
release: CXXFLAGS += -O2 -DNDEBUG
release: $(GAME_TARGET) $(LEVEL_EDITOR_TARGET)

# Game target
$(GAME_TARGET): $(OBJS) build/cmd/game/main.o
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $^ $(LIBS) -o $@

# Level editor target
$(LEVEL_EDITOR_TARGET): $(OBJS) build/cmd/level_editor/main.o
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $^ $(LIBS) -o $@

# Rule for compiling main.cpp files
build/cmd/%.o: cmd/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule for compiling other source files
build/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -rf build bin

# Phony targets
.PHONY: all clean debug release
