#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <set>

#include "card.h"

class Player {
private:
    int score;
    std::string name;
    std::set<Card*, Card::compareCards> myCards;
    int numCards;

protected:
    Player();

public:
    Player(std::string);
    std::string GetName() const;

    int GetScore() const;
    int AddScore(int);

    virtual std::set<Card*, Card::compareCards> GetCards() const;
    void PrintCards() const;
    bool DrawCard(Card*);
    Card* DiscardCard(int, Suit);
    Card* DiscardRandomCard();

};

#endif