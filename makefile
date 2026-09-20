SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
TEST_DIR = tests

CXX ?= g++
CXXFLAGS = -Wall -Wextra -std=c++17 -O2 -fdiagnostics-color=always
DEBUG = 
COVERAGEFLAGS = -O0 --coverage

.PHONY: all build buildWithCoverage tests testsWithCoverage coverage clean

# Find all C++ source files
CPP_SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
TEST_ROOT_SOURCES := $(wildcard $(TEST_DIR)/*.cpp)
TEST_SRC_SOURCES := $(wildcard $(TEST_DIR)/$(SRC_DIR)/*.cpp)

# Generate corresponding object file names in the object directory
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CPP_SOURCES))

TEST_ROOT_OBJECTS := $(patsubst $(TEST_DIR)/%.cpp,$(TEST_DIR)/$(OBJ_DIR)/%.o,$(TEST_ROOT_SOURCES))
TEST_SRC_OBJECTS := $(patsubst $(TEST_DIR)/$(SRC_DIR)/%.cpp,$(TEST_DIR)/$(OBJ_DIR)/%.o,$(TEST_SRC_SOURCES))

TEST_OBJECTS := $(filter-out $(OBJ_DIR)/main.o,$(OBJECTS))
TEST_OBJECTS += $(TEST_ROOT_OBJECTS) $(TEST_SRC_OBJECTS)


all: build clean

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(TEST_DIR)/$(OBJ_DIR):
	mkdir -p $@



# Pattern rule to compile .cpp files into .o files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(DEBUG) -c $< -o $@

$(TEST_DIR)/$(OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp | $(TEST_DIR)/$(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(DEBUG) -c $< -o $@

$(TEST_SRC_OBJECTS): $(TEST_DIR)/$(OBJ_DIR)/%.o: $(TEST_DIR)/$(SRC_DIR)/%.cpp | $(TEST_DIR)/$(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(DEBUG) -c $< -o $@

build: $(OBJECTS) | $(BIN_DIR) $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(DEBUG) $^ -o $(BIN_DIR)/Racko.exe

buildWithCoverage: CXXFLAGS += $(COVERAGEFLAGS)
buildWithCoverage: $(OBJECTS) | $(BIN_DIR) $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $(BIN_DIR)/RackoCoverage.exe

tests: $(TEST_OBJECTS) | $(TEST_DIR)/$(OBJ_DIR) $(OBJ_DIR) $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(DEBUG) $(TESTFLAGS) $^ -o $(BIN_DIR)/$@.exe
	./$(BIN_DIR)/$@.exe

testsWithCoverage: CXXFLAGS += $(COVERAGEFLAGS)
testsWithCoverage: $(TEST_OBJECTS) | $(TEST_DIR)/$(OBJ_DIR) $(OBJ_DIR) $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(TESTFLAGS) $^ -o $(BIN_DIR)/testsCoverage.exe
	./$(BIN_DIR)/$@.exe

coverage: clean
	$(MAKE) buildWithCoverage
	$(MAKE) testsWithCoverage
	./$(BIN_DIR)/testsCoverage.exe

clean:
	rm -rf $(OBJ_DIR)/*.o
	rm -rf $(TEST_DIR)/$(OBJ_DIR)/*.o