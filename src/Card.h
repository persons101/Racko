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
    virtual char getValueChar() const;
    Suit getSuit() const;
    std::string getSuitName() const;
    std::string getSuitSymbol() const;
    bool isBlack() { return (int)suit % 2 == 1; }

    virtual void PrintCard() const;

    static const std::string suit_name(Suit suit);
};

#endif