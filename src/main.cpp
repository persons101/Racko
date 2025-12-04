#include <iostream>

#include "deck.h"


int main() {
    Deck deck(60,1);

    deck.PrintDeck();

    deck.Shuffle();

    std::cout << "\nAfter Shuffling:\n";
    deck.PrintDeck();

    return 0;
}