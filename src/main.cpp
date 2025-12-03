#include <iostream>

#include "deck.h"


int main() {
    Deck deck;

    deck.PrintDeck();

    deck.Shuffle();

    std::cout << "\nAfter Shuffling:\n";
    deck.PrintDeck();

    return 0;
}