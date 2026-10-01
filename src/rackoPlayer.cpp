#include "rackoPlayer.h"

#include <iostream>
#include <string>
#include <limits>

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

std::string RackoPlayer::PrintCards() const
{
    std::vector<int> cardVals = GetCardVals();
    const int cardValsSize = cardVals.size();

    std::string cardLine = "Card: ";
    std::string posLine = "Idx:  ";
    std::string bothLines = "";
    int width = 4;

    for (std::size_t i = 0; i < cardValsSize; i++) {
        cardLine += center(std::to_string(cardVals.at(i)), width);
        posLine += center(std::to_string(5*(i+1)), width);
    }

    bothLines = cardLine + "\n" + posLine;

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
    if (cardToReplace == nullptr) {
        return -1;
    }

    int idx = -1;
    std::string input = "";
    std::cout << PrintCards() << "\n";
    std::cout << "Choose a card to remove for " << cardToReplace << " (e.g. '15'): ";
    do
    {
        if (!(std::cin >> input)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        try {
            idx = stoi(input);
            idx /= 5;   // Convert from score value idx to regular idx
            idx--;      // 1-indexed -> 0-indexed
        }
        catch (const std::invalid_argument& e) {
            std::cerr << "Invalid argument: The string does not begin with a valid number. " << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } 
        catch (const std::out_of_range& e) {
            std::cerr << "Out of range: The value is too large or too small for an int." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

    } while (!(idx >= 0 && idx < (GetNumCards())));
    

    return idx;
}

Card *RackoPlayer::ReplaceCard(Card * newCard, int pos)
{
    if (newCard == nullptr ||
        pos < 0 ||
        static_cast<std::size_t>(pos) >= myCards.size()) {
        return nullptr;
    }
    
    Card* oldCard = myCards.at(pos);
    myCards.at(pos) = newCard;

    return oldCard;
}
