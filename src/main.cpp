#include <iostream>

#include "deck.h"
#include "player.h"

int main() {
    Deck deck(60,1);
    Player player1("Steve");

    for (int suit = 1; suit <= 4; suit++)
        for (int cardVal = 1; cardVal <= 13; cardVal++)
            player1.DrawCard(new Card(cardVal, (Suit)suit));

    player1.PrintCards();

    for (int suit = 1; suit <= 4; suit++)
        for (int cardVal = 1; cardVal <= 13; cardVal++)
            player1.DiscardRandomCard();

    player1.PrintCards();

    deck.PrintDeck();

    for (int suit = 1; suit <= 4; suit++)
        for (int cardVal = 1; cardVal <= 13; cardVal++)
            player1.DrawCard(deck.GetTopCard());

    deck.PrintDeck();
    player1.PrintCards();

    return 0;
}