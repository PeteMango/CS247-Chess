CXX = g++
CXXFLAGS = -std=c++14 -Wall -g
EXEC = out/chess
TEST_EXEC = out/run_tests
SRC_DIR = src
TEST_DIR = tests
OUT_DIR = build
CFILES = $(wildcard $(SRC_DIR)/*.cpp)
TEST_FILES = $(wildcard $(TEST_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OUT_DIR)/%.o,$(CFILES))
TEST_OBJECTS = $(patsubst $(TEST_DIR)/%.cpp,$(OUT_DIR)/test_%.o,$(TEST_FILES))
TEST_DEPS = $(TEST_OBJECTS:.o=.d)
SRC_DEPS = $(OBJECTS:.o=.d)

all: $(EXEC) $(TEST_EXEC)

$(EXEC): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(EXEC)

$(TEST_EXEC): $(TEST_OBJECTS)
	$(CXX) $(TEST_OBJECTS) -lgtest -lgtest_main -pthread -o $(TEST_EXEC)

-include $(SRC_DEPS)
-include $(TEST_DEPS)

$(OUT_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OUT_DIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(OUT_DIR)/test_%.o: $(TEST_DIR)/%.cpp | $(OUT_DIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

.PHONY: clean
clean:
	rm -rf $(OUT_DIR)
