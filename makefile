SRC_DIR = ./src
OBJ_DIR = ./obj
BIN_DIR = ./bin

CXX=g++
CXXFLAGS = -Wall -Wextra -std=c++17 -O2
INCLUDES = -I$(SRC_DIR)

objects = card.o deck.o

.PHONY: all build clean

all: build clean

bin:
	mkdir -p bin

obj:
	mkdir -p obj

build: $(SRC_DIR)/*.cpp | bin obj
		$(CXX) $(CXXFLAGS) $^ -o $(BIN_DIR)/Racko.exe

# $(objects:%=$(OBJ_DIR)/%): $(objects:%.o=$(SRC_DIR)/%.cpp)
# 	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@



$(OBJ_DIR)/deck.o: $(SRC_DIR)/deck.cpp 
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/card.o: $(SRC_DIR)/card.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf obj/*.o