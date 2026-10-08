#include "testPlayer.h"

#include "../../src/rackoCard.h"

void TestPlayer::SetCards() {
    std::vector<int> myCardVals = GetTestCardVals();
    myCards = {};
    for (auto val : myCardVals) {
        Card* currCard = new RackoCard(val);
        DrawCard(currCard);
    }
    
}

TestPlayer::TestPlayer() : RackoPlayer("Test") {
    SetCards();
}

TestPlayer::TestPlayer(std::string newName) : RackoPlayer(newName) {
    SetCards();
}

void TestPlayer::SetCards(const std::vector<Card *>& cardsToAdd)
{
    ClearCards();
    for (auto card : cardsToAdd) {
        DrawCard(card);
    }
}

void TestPlayer::SetCards(const std::vector<int>& valsToAdd)
{
    ClearCards();
    for (auto val : valsToAdd) {
        DrawCard(new RackoCard(val));
    }
}

void TestPlayer::ClearCards()
{
    for (auto* card : myCards) {
        delete card;
    }
    myCards = {};
    numCards = 0;
}

std::vector<int> TestPlayer::GetTestCardVals()
{
    return std::vector<int>({1,5,10,18,20,25,30,35,36,38,40,2,3,4,6 });
}
