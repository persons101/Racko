#pragma once

#include "player.h"

class RackoPlayer : public Player {
protected:
    std::string center(const std::string& text, int width, char padding = ' ') const;

public:
    RackoPlayer(std::string);

	Card* DiscardCard(int cardPos, Suit suit = (Suit)0) override;
    virtual std::string PrintCards() const override;
    virtual bool IsCompletedWithRack();
    int SelectCardToReplace(const Card*) const;
    Card* ReplaceCard(Card*, int pos);

    /// @brief Sorts by score, then alphabetical
    /// @param other Player to compare to
    /// @return True if current player score is less than other score, or if equal true if name is earlier alphabetically 
    bool operator<(const RackoPlayer& other) {
        if (this->GetScore() != other.GetScore()) {
            return this->name < other.name;
        }
        return this->GetScore() < other.GetScore();
    }
};
