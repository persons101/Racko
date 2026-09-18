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
    Card(std::tuple<int, Suit>);
    int getValue() const;
    virtual char getValueChar() const;
    virtual std::string getValueString() const;
    Suit getSuit() const;
    virtual std::string getSuitName() const;
    virtual std::string getSuitSymbol() const;
    virtual bool isBlack() { return (int)suit % 2 == 1; }

    virtual std::string PrintCard() const;
    virtual std::string PrintCardShort() const;

    static const std::string suit_name(Suit);

    auto tied() const;
    virtual bool operator==(const Card& rhs) const;
    inline bool operator!=(const Card& rhs) const { return !(*this == rhs); }; 
    struct compareCards { virtual bool operator() (const Card, const Card) const; 
                          virtual bool operator() (const Card*, const Card*) const; 
                          virtual bool operator() (const Card* lhs, const Card rhs) const { return (lhs < &rhs);  }
                          virtual bool operator() (const Card, const std::tuple<int,Suit>) const; 
                          virtual bool operator() (const Card*, const std::tuple<int,Suit>) const; 
                        };
};

#endif
