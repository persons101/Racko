#pragma once

#include "player.h"

class RackoPlayer : public Player {
protected:
    std::string center(const std::string& text, int width, char padding = ' ') const;

public:
    RackoPlayer(std::string);

	Card* DiscardCard(int cardPos, Suit suit = (Suit)0) override;
    virtual std::string DisplayCardsInRack() const;
    virtual bool IsCompletedWithRack();
    int SelectCardToReplace(const Card*) const;
    Card* ReplaceCard(Card*, int pos);
};
