// Card.h
// Implementation of a Card class for use in a card game
#ifndef CARD_H
#define CARD_H

#include <string>

enum Suit {
    SPADES = 1,
    HEARTS = 2,
    DIAMONDS = 3,
    CLUBS = 4,
};



class Card {
private:
    int value;
    Suit suit;

    
public:
    Card(int, Suit);
    int getValue() const;
    Suit getSuit() const;
    std::string getSuitName() const;
    bool isBlack() { return (int)suit % 2 == 1; }

    static const std::string suit_name(Suit suit);
};

#endif