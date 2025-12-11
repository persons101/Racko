#include "rackoDeck.h"

#include "card.h"

RackoDeck::RackoDeck() : Deck(0, 0, 0) { 
    AddCardsToDeck(60, 1);
}

Card* RackoDeck::MakeNewCard(int cardVal, Suit cardSuit) {
    return new RackoCard(cardVal);
}