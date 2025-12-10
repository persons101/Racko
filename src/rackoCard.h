#include "card.h"

class RackoCard : Card {
    virtual std::string getSuitSymbol() const override;
    virtual void PrintCardShort() const override;
};