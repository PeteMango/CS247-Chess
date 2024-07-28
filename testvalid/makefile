# Compiler
CXX := g++

# Directories
SRC_DIR := ../src
INCLUDE_DIR := ../include
TESTVALID_DIR := .

# Source Files
SRC_FILES := $(shell find $(SRC_DIR) -name '*.cpp')
TESTVALID_FILES := $(shell find $(TESTVALID_DIR) -name '*.cpp')

# Exclude main.cpp from src directory
SRC_FILES := $(filter-out $(SRC_DIR)/main.cpp, $(SRC_FILES))

# Object Files
OBJ_FILES := $(SRC_FILES:.cpp=.o) $(TESTVALID_FILES:.cpp=.o)

# Executable
EXECUTABLE := testmoves

# Compiler Flags
CXXFLAGS := -I$(INCLUDE_DIR) -I$(TESTVALID_DIR) -std=c++17 -Wall

# Linking Flags
LDFLAGS := -lX11 -lcurl

# Rules
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ_FILES)
	$(CXX) $(OBJ_FILES) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_FILES) $(EXECUTABLE)

.PHONY: all clean
