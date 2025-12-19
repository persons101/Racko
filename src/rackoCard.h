#ifndef RACKOCARD_H
#define RACKOCARD_H

#include "card.h"

class RackoCard : public Card {
public:
    RackoCard(int cardVal);
    RackoCard(Card card);
    virtual std::string getSuitSymbol() const override;
};

#endif