CXX = g++
CXXFLAGS = -std=c++14 -Wall -g
EXEC = out/chess
TEST_EXEC = out/run_tests
SRC_DIR = src
TEST_DIR = tests
OUT_DIR = build
CFILES = $(shell find $(SRC_DIR) -name '*.cpp')
TEST_FILES = $(shell find $(TEST_DIR) -name '*.cpp')
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OUT_DIR)/%.o,$(CFILES))
TEST_OBJECTS = $(patsubst $(TEST_DIR)/%.cpp,$(OUT_DIR)/test_%.o,$(TEST_FILES))
TEST_DEPS = $(TEST_OBJECTS:.o=.d)
SRC_DEPS = $(OBJECTS:.o=.d)
DIRS = $(sort $(dir $(OBJECTS)) $(dir $(TEST_OBJECTS)))

all: $(EXEC) $(TEST_EXEC)

$(EXEC): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(EXEC)

$(TEST_EXEC): $(TEST_OBJECTS)
	$(CXX) $(TEST_OBJECTS) -lgtest -lgtest_main -pthread -o $(TEST_EXEC)

-include $(SRC_DEPS)
-include $(TEST_DEPS)

$(OUT_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OUT_DIR)/dirs
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(OUT_DIR)/test_%.o: $(TEST_DIR)/%.cpp | $(OUT_DIR)/dirs
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(OUT_DIR)/dirs:
	mkdir -p $(DIRS)

.PHONY: clean
clean:
	rm -rf $(OUT_DIR)/*
