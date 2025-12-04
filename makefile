SRC_DIR = ./src
OBJ_DIR = ./obj
BIN_DIR = ./bin
TEST_DIR = ./tests

CXX=g++
CXXFLAGS = -Wall -Wextra -std=c++17 -O2

objects = card.o deck.o

.PHONY: all build tests clean

all: build clean

bin:
	mkdir -p bin

obj:
	mkdir -p obj

build: $(SRC_DIR)/*.cpp | bin obj
	$(CXX) $(CXXFLAGS) $^ -o $(BIN_DIR)/Racko.exe

$(OBJ_DIR)/deck.o: $(SRC_DIR)/deck.cpp 
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/card.o: $(SRC_DIR)/card.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

tests: $(TEST_DIR)/catch_amalgamated.cpp $(TEST_DIR)/tests.cpp
	$(CXX) $(CXXFLAGS) $^ -o $(BIN_DIR)/$@.exe

clean:
	rm -rf obj/*.o