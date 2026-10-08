#ifndef RACKOCARD_H
#define RACKOCARD_H

#include "card.h"

class RackoCard : public Card {
public:
    RackoCard(int cardVal);
    RackoCard(const Card& card);
    ~RackoCard() = default;
    virtual Card* getCopy() const override;
    virtual std::string getSuitSymbol() const override;
    virtual std::string PrintCard() const override;
    virtual std::string PrintCardShort() const override;
    friend std::ostream& operator<<(std::ostream& os, const RackoCard&);
};

#endif