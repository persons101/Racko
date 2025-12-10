// Deck.h
// Implementation of the Deck class for a card game

#ifndef DECK_H
#define DECK_H

#include <vector>

#include "card.h"


class Deck {
private:
    std::vector<Card*> deck;
    int numCards;
    int numSuits;

protected:
    virtual Card* MakeNewCard(int, Suit);
    virtual void AddCardsToDeck(int numCardsPerSuit, int numSuits);
    virtual void AddJokersToDeck(int numJokers, int numSuits);

public:
    Deck();
    Deck(int numCardsPerSuit, int numSuits);
    Deck(int numCardsPerSuit, int numSuits, int numJokers);
    void Shuffle();
    Card* GetTopCard();
    Card* GetBottomCard();
    int GetNumCards() const { return numCards;}
    void PrintDeck() const;
};


#endif
