CXX = g++
CXXFLAGS = -std=c++14 -Wall -g -Iinclude
EXEC = out/chess
TEST_EXEC = out/run_tests
SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build
OUT_DIR = out
CFILES = $(shell find $(SRC_DIR) -name '*.cpp')
TEST_FILES = $(shell find $(TEST_DIR) -name '*.cpp')
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(CFILES))
TEST_OBJECTS = $(patsubst $(TEST_DIR)/%.cpp,$(BUILD_DIR)/test_%.o,$(TEST_FILES))
TEST_DEPS = $(TEST_OBJECTS:.o=.d)
SRC_DEPS = $(OBJECTS:.o=.d)
DIRS = $(sort $(dir $(OBJECTS)) $(dir $(TEST_OBJECTS)))

all: $(EXEC) $(TEST_EXEC)

$(EXEC): $(OBJECTS) | $(OUT_DIR)
	$(CXX) $(OBJECTS) -o $(EXEC) -lX11

$(TEST_EXEC): $(TEST_OBJECTS) | $(OUT_DIR)
	$(CXX) $(TEST_OBJECTS) -lgtest -lgtest_main -pthread -o $(TEST_EXEC)

-include $(SRC_DEPS)
-include $(TEST_DEPS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)/dirs
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/test_%.o: $(TEST_DIR)/%.cpp | $(BUILD_DIR)/dirs
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/dirs:
	mkdir -p $(DIRS)

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/*
	rm -rf $(OUT_DIR)/*
