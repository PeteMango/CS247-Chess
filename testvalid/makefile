# Define variables
SRC_DIR := ../src
INC_DIR := ../include
TEST_DIR := .
OBJ_DIR := obj

# List of source files in src directory, excluding main.cpp
SRC_FILES := $(filter-out $(SRC_DIR)/main.cpp, $(wildcard $(SRC_DIR)/*.cpp))
TEST_MAIN := $(TEST_DIR)/main.cpp

# Object files
OBJ_FILES := $(SRC_FILES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
TEST_OBJ := $(TEST_MAIN:$(TEST_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Compiler and flags
CXX := g++
CXXFLAGS := -I$(INC_DIR) -Wall -Wextra -std=c++17

# Target executable
TARGET := test_program

# Default target
all: $(TARGET)

# Create obj directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Rule to build the target executable
$(TARGET): $(OBJ_FILES) $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to build object files from source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to build the test main object file
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean
