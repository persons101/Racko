CXX=g++
CXXFLAGS = -Wall -Wextra -std=c++17 -O2
INCLUDES = -I./src

objects = Card.o Deck.o Suit.o

SRC_DIR = ./src
OBJ_DIR = ./obj
BIN_DIR = ./bin

.PHONY: all clean

all: $(BIN_DIR)/Racko.exe clean

bin:
	mkdir -p bin

obj:
	mkdir -p obj

$(BIN_DIR)/Racko.exe: $(SRC_DIR)/main.cpp $(objects:%=$(OBJ_DIR)/%) | bin obj
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@ 
	
$(objects:%=$(OBJ_DIR)/%): $(objects:%.o=$(SRC_DIR)/%.cpp)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf obj/*.o