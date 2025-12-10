#include "rackoDeck.h"

#include "card.h"

RackoDeck::RackoDeck() : Deck(60, 1, 0) { }

Card* RackoDeck::MakeNewCard(int cardVal, Suit cardSuit) {
    return new RackoCard(cardVal);
}