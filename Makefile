CXX = g++
CXXFLAGS = -std=c++14 -Wall -g -Iinclude
EXEC = out/chess
SRC_DIR = src
BUILD_DIR = build
OUT_DIR = out
CFILES = $(shell find $(SRC_DIR) -name '*.cpp')
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(CFILES))
SRC_DEPS = $(OBJECTS:.o=.d)
DIRS = $(sort $(dir $(OBJECTS)))

all: $(EXEC)

$(EXEC): $(OBJECTS) | $(OUT_DIR)
	$(CXX) $(OBJECTS) -o $(EXEC) -lX11

-include $(SRC_DEPS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)/dirs
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/dirs:
	mkdir -p $(DIRS)

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/*
	rm -rf $(OUT_DIR)/*
