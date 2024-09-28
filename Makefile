# Compiler and flags
CXX = clang++
CXXFLAGS = -std=c++20 -Iinclude -Ilib -Wall -Wextra -Werror

# Raylib library and framework dependencies
LIBS = -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL lib/libraylib.a

# Automatically find all source files in the src directory and subdirectories
SRCS = $(shell find src -type f -name "*.cpp")

# Convert source files into corresponding object files in the build directory
OBJS = $(patsubst src/%.cpp, build/%.o, $(SRCS))

# Target binary
TARGET = bin/game

# Default build is a debug build
all: debug

# Debug build
debug: CXXFLAGS += -g -O0  # Debug symbols, no optimization
debug: $(TARGET)

# Release build
release: CXXFLAGS += -O2 -DNDEBUG  # Optimization, disable assertions
release: $(TARGET)

# Linking the object files to create the binary
$(TARGET): $(OBJS)
	@mkdir -p bin  # Ensure bin directory exists
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBS) -o $(TARGET)

# Rule to compile individual source files into object files
# This also ensures the necessary directories in `build/` are created
build/%.o: src/%.cpp
	@mkdir -p $(dir $@)  # Ensure the subdirectories in `build/` exist
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule to remove the compiled binary and object files
clean:
	find ./bin -type f -delete
	find ./build/ -type f ! -name 'compile_commands.json' -delete
	find ./build/ -type d  -delete

# Mark the `all`, `clean`, `debug`, and `release` as phony targets
.PHONY: all clean debug release
