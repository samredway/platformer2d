# Compiler and flags
CXX = clang++
CXXFLAGS = -std=c++20 -Iinclude -Ilib -Wall -Wextra -Werror

# Raylib library and framework dependencies
LIBS = -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL lib/libraylib.a

# Source files
SRCS = $(shell find src -type f -name "*.cpp")

# Object files
OBJS = $(patsubst src/%.cpp, build/%.o, $(SRCS))

# Target
GAME_TARGET = bin/game

# Default build is a debug build
all: debug

# Debug build
debug: CXXFLAGS += -g -O0
debug: $(GAME_TARGET)

# Release build
release: CXXFLAGS += -O2 -DNDEBUG
release: $(GAME_TARGET)

# Game target
$(GAME_TARGET): $(OBJS)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $^ $(LIBS) -o $@

# Rule for compiling source files
build/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -rf build bin

# Phony targets
.PHONY: all clean debug release
