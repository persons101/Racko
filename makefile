SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
TEST_DIR = tests

CXX ?= g++
CXXFLAGS = -Wall -Wextra -std=c++17 -O2 -fdiagnostics-color=always
DEBUG = 

# Find all C++ source files
CPP_SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
TEST_SOURCES := $(wildcard $(TEST_DIR)/*.cpp)

# Generate corresponding object file names in the object directory
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CPP_SOURCES))

TEST_OBJECTS = $(filter-out $(OBJ_DIR)/main.o, $(OBJECTS))
TEST_OBJECTS += $(patsubst $(TEST_DIR)/%.cpp, $(TEST_DIR)/$(OBJ_DIR)/%.o, $(TEST_SOURCES))

.PHONY: all build tests clean

all: build clean

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(TEST_DIR)/$(OBJ_DIR):
	mkdir -p $@

# build: $(SRC_DIR)/*.cpp | bin obj
# 	$(CXX) $(CXXFLAGS) $^ -o $(BIN_DIR)/Racko.exe

# Pattern rule to compile .cpp files into .o files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(DEBUG) -c $< -o $@

$(TEST_DIR)/$(OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp | $(TEST_DIR)/$(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(DEBUG) -c $< -o $@

build: $(OBJECTS) | $(BIN_DIR) $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(DEBUG) $^ -o $(BIN_DIR)/Racko.exe


tests: $(TEST_OBJECTS) | $(TEST_DIR)/$(OBJ_DIR) $(OBJ_DIR) $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(DEBUG) $^ -o $(BIN_DIR)/$@.exe

testAndRun: build tests
	./$(BIN_DIR)/tests.exe

clean:
	rm -rf $(OBJ_DIR)/*.o
	rm -rf $(TEST_DIR)/$(OBJ_DIR)/*.o