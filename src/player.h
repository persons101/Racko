#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>

#include "card.h"

class Player {
private:
    int score;
    
protected:
    Player();
    int numCards;
    std::string name;
    std::vector<Card*> myCards;
public:
    Player(std::string);
    ~Player();

    std::string GetName() const;
    int GetNumCards() const;    

    int GetScore() const;
    int AddScore(int);
    
    std::vector<Card*> ResetPlayer(bool AddToDeck = false);
    
    virtual std::vector<Card*> GetCards() const;
    std::vector<int> GetCardVals() const;
    virtual std::string PrintCards() const;
    bool DrawCard(Card*);
    virtual Card* DiscardCard(int, Suit);
    Card* DiscardRandomCard();

};

#endif