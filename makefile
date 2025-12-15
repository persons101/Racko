SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
TEST_DIR = tests

CXX=g++
CXXFLAGS = -Wall -Wextra -std=c++17 -O2 -fdiagnostics-color=always
DEBUG = 

objects = card.o deck.o

.PHONY: all build tests clean

all: build clean

bin:
	mkdir -p bin

obj:
	mkdir -p obj

# build: $(SRC_DIR)/*.cpp | bin obj
# 	$(CXX) $(CXXFLAGS) $^ -o $(BIN_DIR)/Racko.exe

# Find all C++ source files
CPP_SOURCES := $(wildcard $(SRC_DIR)/*.cpp)

# Generate corresponding object file names in the object directory
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CPP_SOURCES))
OBJECTSNOMAIN := $(filter-out $(OBJ_DIR)/main.o, $(OBJECTS))
# Pattern rule to compile .cpp files into .o files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR) # Ensure the object directory exists
	gcc -c $< -o $@

build: $(OBJECTS) | bin obj
	$(CXX) $(CXXFLAGS) $(DEBUG) $^ -o $(BIN_DIR)/Racko.exe


tests: $(TEST_DIR)/catch_amalgamated.cpp $(TEST_DIR)/tests.cpp $(OBJECTSNOMAIN)
	$(CXX) $(CXXFLAGS) $(DEBUG) $^ -o $(BIN_DIR)/$@.exe

testAndRun:	 build tests
	./$(BIN_DIR)/tests.exe

clean:
	rm -rf obj/*.o