#include <iostream>

#include "rackoDeck.h"
#include "player.h"

int main() {
    RackoDeck deck;
    Player player1("Steve");

    while (player1.DrawCard(deck.GetTopCard()));

    player1.PrintCards();

    

    return 0;
}