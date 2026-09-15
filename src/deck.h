// Deck.h
// Implementation of the Deck class for a card game

#ifndef DECK_H
#define DECK_H

#include <vector>

#include "card.h"


class Deck {
private:
    int numCards;
    int numSuits;

protected:
    std::vector<Card*> deck;
    virtual Card* MakeNewCard(int, Suit);
    virtual Card* MakeNewCard(Card*);
    virtual void AddCardsToDeck(int numCardsPerSuit, int numSuits);
    virtual void AddJokersToDeck(int numJokers);

public:
    Deck();
    ~Deck();
    Deck(int numCardsPerSuit, int numSuits);
    Deck(int numCardsPerSuit, int numSuits, int numJokers);
    void Shuffle();
    Card* GetTopCard();
    Card* GetBottomCard();
    int GetNumCards() const { return numCards;}
    std::vector<Card*> GetCards() const { return deck; }
    virtual void PrintDeck() const;
};


#endif
