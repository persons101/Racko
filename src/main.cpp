#include <iostream>

#include "deck.h"
#include "player.h"

int main() {
    Deck deck(60,4);
    Player player1("Steve");

    for (int i = 1; i <= 4; i++)
        player1.DrawCard(new Card(1, (Suit)i));

    player1.PrintCards();

    return 0;
}