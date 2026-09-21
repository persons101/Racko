#include "rackoPlayer.h"

#include <iostream>
#include <string>

// Helper function to return a center-padded string
std::string RackoPlayer::center(const std::string &text, int width, char padding) const
 {
    if (text.length() >= width) {
        return text; // Return as-is if it's wider than the slot
    }
    
    int total_padding = width - text.length();
    int left_padding = total_padding / 2;
    int right_padding = total_padding - left_padding;
    
    return std::string(left_padding, padding) + text + std::string(right_padding, padding);
}

RackoPlayer::RackoPlayer(std::string newName) : Player(newName) {}

Card* RackoPlayer::DiscardCard(int cardPos, Suit suit)
{
    try {
	    Card* discardedCard = myCards.at(cardPos);
        myCards.erase(myCards.begin() + cardPos);
	    numCards--;
        return discardedCard;
    }
    catch (const std::invalid_argument& e) {
            std::cerr << "Invalid argument: cardPos is not a valid number. " << std::endl;
        } 
    catch (const std::out_of_range& e) {
        std::cerr << "Out of range: The value is too large or too small for an int." << std::endl;
    }
	
    return nullptr;
}

std::string RackoPlayer::DisplayCardsInRack() const
{
    std::vector<int> cardVals = GetCardVals();
    const int numCards = cardVals.size();

    std::string cardLine = "";
    std::string valueLine = "";
    std::string bothLines = "";
    int width = 4;

    for (std::size_t i = 0; i < numCards; i++) {
        cardLine += center(std::to_string(cardVals.at(i)), width);
        valueLine += center(std::to_string(5*i), width);
    }

    bothLines = cardLine + "\n" + valueLine;

    return bothLines;
}

bool RackoPlayer::IsCompletedWithRack()
{
    for (int i = 0; i < myCards.size() - 1; i++) {
        if (!(myCards.at(i + 1)->getValue() > myCards.at(i)->getValue()))
            return false;
    }

    return true;
}

int RackoPlayer::SelectCardToReplace(const Card * cardToReplace) const
{
    int idx = -1;
    std::string input = "";
    std::cout << DisplayCardsInRack() << "\n";
    std::cout << "Choose a card to remove (e.g. '15'): ";
    do
    {
        try {
            std::cin >> input;

            idx = std::stoi(input); 
            idx /= 5; // number -> index
            idx--; // convert 1-indexed to 0-indexed
        }
        catch (const std::invalid_argument& e) {
            std::cerr << "Invalid argument: The string does not begin with a valid number. " << std::endl;
        } 
        catch (const std::out_of_range& e) {
            std::cerr << "Out of range: The value is too large or too small for an int." << std::endl;
        }

    } while (!(idx >= 0 && idx < (GetNumCards())));
    

    return idx;
}

Card *RackoPlayer::ReplaceCard(Card * newCard, int pos)
{
    if (newCard == nullptr ||
        pos < 0 ||
        static_cast<std::size_t>(pos) >= GetNumCards()) {
        return nullptr;
    }
    
    Card* oldCard = myCards.at(pos);
    myCards.at(pos) = newCard;

    return oldCard;
}
