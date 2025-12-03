SRC_DIR = ./src
OBJ_DIR = ./obj
BIN_DIR = ./bin

CXX=g++
CXXFLAGS = -Wall -Wextra -std=c++17 -O2
INCLUDES = -I$(SRC_DIR)

objects = Suit.o Card.o Deck.o

.PHONY: all clean

all: $(BIN_DIR)/Racko.exe clean

bin:
	mkdir -p bin

obj:
	mkdir -p obj

$(BIN_DIR)/Racko.exe: $(SRC_DIR)/main.cpp $(objects:%=$(OBJ_DIR)/%) | bin obj
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@ 

deck.o: deck.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@ 

$(objects:%=$(OBJ_DIR)/%): $(objects:%.o=$(SRC_DIR)/%.cpp)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf obj/*.o