#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>

#include "card.h"

class Player {
private:
    int score;
    int numCards;

protected:
    Player();
    std::string name;
    std::vector<Card*> myCards;
public:
    Player(std::string);
    ~Player();
    std::string GetName() const;

    int GetScore() const;
    int AddScore(int);
    
    virtual std::vector<Card*> GetCards() const;
    std::vector<int> GetCardVals() const;
    virtual void PrintCards() const;
    bool DrawCard(Card*);
    virtual Card* DiscardCard(int, Suit);
    Card* DiscardRandomCard();

};

#endif