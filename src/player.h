#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>

#include "card.h"

class Player {
private:
    int score;
    std::string name;
    std::vector<Card*> myCards;

protected:
    Player();

public:
    Player(std::string);
    std::string GetName() const;

    int GetScore() const;
    int AddScore(int);

    virtual std::vector<Card*> GetCards() const;
    bool DrawCard(Card*);
    Card* DiscardCard(Card*);
};

#endif