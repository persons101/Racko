#include "rackoDeck.h"

#include <iostream>
#include "card.h"

RackoDeck::RackoDeck() : Deck(0, 0, 0) { 
    AddCardsToDeck(60, 1);
}

void RackoDeck::PrintDeck() const
{
    std::cout << "---Start Deck---\n";
    for (const auto& card : GetCards()) {
        card->PrintCardShort();
    }
    std::cout << "\n";
    std::cout << "--- End Deck ---\n";
}

Card* RackoDeck::MakeNewCard(int cardVal, Suit cardSuit) {
    return new RackoCard(cardVal);
}