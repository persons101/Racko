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
    virtual std::string getValueString() const;
    Suit getSuit() const;
    virtual std::string getSuitName() const;
    virtual std::string getSuitSymbol() const;
    virtual bool isBlack() { return (int)suit % 2 == 1; }

    virtual void PrintCard() const;
    virtual void PrintCardShort() const;

    static const std::string suit_name(Suit);
    struct compareCards { virtual bool operator() (Card, Card) const; 
                          virtual bool operator() (Card*, Card*) const; };
};

#endif
