#include "racko.h"

int Racko::CalculateRackScore(std::vector<Card *>& cards) const
{
    int score = 0;
    const int RACKO_BONUS_REQ = 75;
    const int RACKO_BONUS = 25;

    for (int i = 0; i < cards.size() - 1; i++) {
        if (cards.at(1 + i) > cards.at(i))
            score += 5;
        else 
            break;
    }

    if (score >= RACKO_BONUS_REQ)
        score += RACKO_BONUS;

    return score;
}

int Racko::GetNumTurns() const
{
    return numTurns;
}
