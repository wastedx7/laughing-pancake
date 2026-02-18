CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude

SRC_DIR = src
OBJ_DIR = build
TARGET = $(OBJ_DIR)/app

SRC = $(wildcard $(SRC_DIR)/*.cxx)

OBJ = $(patsubst $(SRC_DIR)/%.cxx,$(OBJ_DIR)/%.o,$(SRC))

all: $(OBJ_DIR) $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

rebuild: clean all

.PHONY: all clean rebuild
