#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <unordered_set>

#include "card.h"

class Player {
private:
    int score;
    std::string name;
    std::unordered_set<Card*> myCards;

protected:
    Player();

public:
    Player(std::string);
    std::string GetName() const;

    int GetScore() const;
    int AddScore(int);

    virtual std::unordered_set<Card*> GetCards() const;
    bool DrawCard(Card*);
    Card* DiscardCard(Card*);
    Card* DiscardRandomCard();
};

#endif