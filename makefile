# Compiler
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude

# Directories
SRC_DIR = src
BUILD_DIR = build

# Target executable
TARGET = $(BUILD_DIR)/app

# Find all source files (.cxx)
SRCS = $(wildcard $(SRC_DIR)/*.cxx)

# Convert src/file.cxx -> build/file.o
OBJS = $(SRCS:$(SRC_DIR)/%.cxx=$(BUILD_DIR)/%.o)

# Default target
all: $(TARGET)

# Link object files into executable
$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(OBJS) -o $(TARGET)

# Compile source files into object files in build/
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cxx
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build directory
clean:
	rm -rf $(BUILD_DIR)

# Rebuild everything
rebuild: clean all

.PHONY: all clean rebuild
