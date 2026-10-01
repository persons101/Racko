#include "racko.h"

#include <iostream>
#include <string>
#include <limits>
#include <exception>
#include "rackoPlayer.h"
#include "rackoDeck.h"

int Racko::CalculateRackScore(const std::vector<Card *>& cards) const
{
    if (cards.size() == 0) {
        return 0;
    }

    int score = 5;

    for (std::size_t i = 0; i < cards.size() - 1; i++) {
        if (cards.at(1 + i)->getValue() > cards.at(i)->getValue()) {
            score += 5;
        }
        else {
            break;
        }
    }
                                                                                                                                                                                 
    if (score >= (RACKO_BONUS_REQ + racko_bonus_req_handicap))
        score += (RACKO_BONUS + racko_bonus_handicap);

    return score;
}

void Racko::AddStartingCardsToPlayer(Player*& player)
{
    for (int i = 0; i < 10; i++) {
        player->DrawCard( PopCard(false) );        
    }
}

char Racko::SelectDrawCard(int playerIdx) const
{
    /// Shows the player at @param{playerIdx} the top of the draw and discard piles
    /// @return D'r'aw pile, D'i'scard, or 'e'rror

    return SelectDrawCard(GetPlayerByIdx(playerIdx));
}

char Racko::SelectDrawCard(Player * player) const
{
    /// Shows the player @param{player} the top of the draw and discard piles
    /// @return D'r'aw pile, D'i'scard, or 'e'rror
    if (player == nullptr || deck->GetTopCard() == nullptr) {
        return 'e';
    }
    std::string input = "";

    std::cout << player->PrintCards() << "\n";
    std::cout << "Deck (0): " << deck->GetTopCard()->PrintCardShort() << ", Discard (1): " << ((!discardPile.empty()) ? discardPile.top()->PrintCardShort() : "-") << "\n";
    std::cout << "Choose a card: Deck='0', Discard='1': ";
    
    // input validation
    while (input != "0" && input != "1") {
        std::cin >> input;
        if (input.size() > 1) {
            input = "-";
        }
        else if (discardPile.empty() && input == "1"){
            std::cout << "Uh-oh, there are no cards in the discard pile to draw! Try again: ";
            input = "-";
        }

        if ( !(input == "0" || input == "1")) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } 
    }

    if (input == "1") {
        return 'i';
    } 

    return 'r';
}

Card* Racko::PopCard(bool isDiscardPileChosen)
{
    Card* card = nullptr;
    try {
        switch (isDiscardPileChosen) {
            case false:
                // draw from deck
                card = deck->PopTopCard();
                cardsInDeck--;
                break;
            case true:
                // draw from discard
                // TODO create discardPile.h/.cpp to handle stack manipulation
                if (discardPile.empty())
                    return nullptr;

                card = discardPile.top();
                discardPile.pop();
                cardsInDiscard--;
                break;
            default:
                // should never run
                throw std::out_of_range("Switch-case failure");
        }
        if (card == nullptr) {
            throw std::out_of_range("No card found in stack!");
        }
    }
    catch (const std::out_of_range& e) {
        std::cerr << e.what() << std::endl;
        return nullptr;
    }

    return card;
}

int Racko::DrawCardForPlayer(int playerIdx, bool isDiscardPileChosen) {
    return DrawCardForPlayer(GetPlayerByIdx(playerIdx), isDiscardPileChosen);
}
    
int Racko::DrawCardForPlayer(Player* player, bool isDiscardPileChosen) {
    if (player == nullptr) {
        return -1;
    }

    Card* cardToDraw = PopCard(isDiscardPileChosen);
    player->DrawCard(cardToDraw);
}

int Racko::SelectCardIdxToDiscard(int playerIdx, const Card* cardToReplace)
{
    return SelectCardIdxToDiscard(GetPlayerByIdx(playerIdx), cardToReplace);
}

int Racko::SelectCardIdxToDiscard(Player * player, const Card* cardToReplace)
{
    if (player == nullptr) {
        return -1;
    }

    if (RackoPlayer* playerR = dynamic_cast<RackoPlayer*>(player) ) {
        return playerR->SelectCardToReplace(cardToReplace);
    }
    else {
        std::string replaceStr = (cardToReplace != nullptr ? " for " + cardToReplace->PrintCardShort() : "");

        std::cout << player->PrintCards();
        std::cout << "Select card to discard using the # beneath it" << replaceStr << ": ";

        // input validation
        std::string input = "";
        int inputNum = -1;
        while (!(inputNum % 5 == 0 && inputNum >= 5 && inputNum <= 50)) {
            if (!(std::cin >> input)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
            try {
                inputNum = stoi(input);
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
        }

        return (inputNum / 5) - 1;
    }
    return -2;
}

int Racko::AddCardToDiscard(Card * cardToAdd)
{
    discardPile.push(cardToAdd);
    cardsInDiscard++;
    return cardsInDiscard;
}

int Racko::GetPlayerIdxByName(std::string playerName) const
{
    int idx = -1;

    Player* currPlayer = nullptr;

    std::vector<Player*>::iterator it;

    int i = 0;
    for (i; i < playerCnt; i++) {
        currPlayer = playerVector.at(i);
        if (currPlayer->GetName() == playerName)
            break;
    }

    idx = i;
    return idx;
}

int Racko::PlayTurnForPlayerByIdx(int playerIdx)
{
    Player* player_raw = playerVector.at(playerIdx);
    if (player_raw == nullptr) {
        return -1;
    }

    RackoPlayer* player = dynamic_cast<RackoPlayer*>(player_raw);
    if (!player) {
        return -2;
    }

    // 1. show cards
    // 2. select card to pickup
    // 3. pickup card
    // 4. select card index to replace
    // 5. put card in discard pile
    // 6. check if complete
    // 7. if round over, score all players

    Card* cardDrawn = nullptr;
    while (cardDrawn == nullptr) {
        // 1+2
        char drawSelectionResult = SelectDrawCard(playerIdx);
        
        bool isDiscardPileChosen;
        switch (drawSelectionResult) {
            case 'r':
                isDiscardPileChosen = false;
                break;
            case 'i':
                isDiscardPileChosen = true;
                break;
            default:
                return -3;
        }

        // 3
        cardDrawn = PopCard(isDiscardPileChosen);
    }
    // 4
    int idxToReplace = SelectCardIdxToDiscard(player, cardDrawn);

    // 5
    int discardStatus = AddCardToDiscard(player->ReplaceCard(cardDrawn, idxToReplace));
    if (discardStatus != 0) {
        return -5;
    }

    // 6
    if (player->IsCompletedWithRack()) {
        int score = CalculateRackScore(player->GetCards());
        return score; // 7 handled by caller
    }
    
    return 0;
}

int Racko::PlayTurnForPlayerByName(std::string playerName)
{
    return PlayTurnForPlayerByIdx(GetPlayerIdxByName(playerName));
}

void Racko::ResetPlayers()
{
    for (Player* player : playerVector) {
        delete player;
    }
    playerVector = {};
    playerCnt = 0;
}

void Racko::AddPlayer(std::string playerName)
{
    Player* newPlayer = new RackoPlayer(playerName);
    playerVector.push_back(newPlayer);
    playerCnt++;
}

void Racko::AddPlayer(Player * newPlayer)
{
    playerVector.push_back(newPlayer);
    playerCnt++;
}

void Racko::SetScoreGoal(int goal)
{
    score_goal = goal;
}

void Racko::SetScoreGoal(Difficulty difficulty)
{
    switch (difficulty) {
        case Difficulty::Easy:
            SetScoreGoal(100);
            break;
        case Difficulty::Medium:
            SetScoreGoal(150);
            break;
        case Difficulty::Hard:
            SetScoreGoal(200);
            break;
        case Difficulty::Custom:
            SetScoreGoal(custom_difficulty_goal_score);
            break;
        default:
            std::cerr << ">>> Error: SetScoreGoal(difficulty) difficulty not implemented!\n";
            SetScoreGoal(150);
    }
}

void Racko::SetDifficulty(Difficulty difficulty)
{
    current_difficulty = difficulty;
}

void Racko::SetCustomGoal(int goal)
{
    custom_difficulty_goal_score = goal;
}

Racko::Racko() : deck(std::make_unique<RackoDeck>())
{
    numGames = 0;   
}

int Racko::Run() {
    int restartVal = 1;
    int winnerIdx = -1;
    
    try {
        do {
            SetupGame();
            winnerIdx = -1;
            do {
                PlayTurn();
                winnerIdx = CheckForWinners();
            } while (winnerIdx < 0);

            restartVal = FinishGame();
            if (restartVal == 1) {
                ResetGame();
            }
        } while (restartVal == 1);
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return winnerIdx;
}

std::size_t Racko::GetNumTurns() const
{
    return numTurns;
}

std::size_t Racko::GetNumGames() const
{
    return numGames;
}

unsigned int Racko::GetPlayerCount() const
{
    return playerCnt;
}

int Racko::GetDeckCardCount() const
{
    return deck->GetNumCards();
}

int Racko::GetDiscardCardCount() const
{
    return discardPile.size();
}

Card *Racko::GetTopCardFromDeck() const
{
    try {
        Card* card = deck->GetTopCard();
        return card;
    }
    catch (const std::out_of_range& e) {
        std::cerr << e.what() << std::endl;
    }

    return nullptr;
}

Card *Racko::GetTopCardFromDiscard() const
{
    if (discardPile.empty()) {
        return nullptr;
    }

    return discardPile.top();
}

std::vector<Card *> Racko::GetPlayerCardsByIdx(int playerIdx) const
{
    Player* player = playerVector.at(playerIdx);
    if (player == nullptr) {
        return std::vector<Card*>();
    }

    std::vector<Card *> cards = player->GetCards();
    return cards;
}

std::vector<Card *> Racko::GetPlayerCardsByName(std::string playerName) const
{
    Player* player = GetPlayerByName(playerName);

    return player->GetCards();
}

Player *Racko::GetPlayerByIdx(int playerIdx) const
{
    try {
        Player* player = playerVector.at(playerIdx);
        return player;
    }
    catch (const std::out_of_range& e) {
        std::cerr << e.what() << std::endl;
    }
    return nullptr;
}

Player *Racko::GetPlayerByName(std::string playerName) const
{
    int idx = GetPlayerIdxByName(playerName);

    if (idx == -1) {
        return nullptr;
    }

    Player* player = GetPlayerByIdx(idx);
    return player;
}

void Racko::ScoreRackByIdx(int playerIdx)
{
    Player* player = GetPlayerByIdx(playerIdx);
    if (!player) {
        return;
    }

    int score = CalculateRackScore(player->GetCards());
    player->AddScore(score);
}

void Racko::ScoreRackByName(std::string playerName)
{
    ScoreRackByIdx(GetPlayerIdxByName(playerName));
}

void Racko::SetupGame()
{
    ResetGame();
    numGames++;

    std::string inputString = "";
    int numPlayersToAdd = 0;
    
    do {
        try {
            std::cout << "Enter number of players: ";
            std::cin >> inputString;

            numPlayersToAdd = std::stoi(inputString);
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
    } while (numPlayersToAdd <= 0);

    for (std::size_t i = 0; i < numPlayersToAdd; i++) {
        Player* player = CreatePlayer();
        AddStartingCardsToPlayer(player);
        AddPlayer(player);
    }

    ChooseDifficulty();
}

Player *Racko::CreatePlayer()
{
    std::string input;
    Player* player;

    do {
        std::cout << "Enter a name for your player: ";
        if (!(std::cin >> input)) {
            return nullptr; // Handle input failure in the caller.
        }
    } while (input.empty());
    
    player = new RackoPlayer(input);

    return player;
}

void Racko::ChooseDifficulty()
{
    Difficulty difficulty;
    std::string inputString;
    int inputSetting = 0;
    std::cout << "Please choose a difficulty: (1)Easy, (2)Medium, (3)Hard, (4)Custom\n";
    do {
        try {
            std::cin >> inputString;

            inputSetting = std::stoi(inputString);
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
    } while (inputSetting <= 0 || inputSetting > 4);

    switch (inputSetting) {
        case 1:
            difficulty = Difficulty::Easy;
            break;
        case 2:
            difficulty = Difficulty::Medium;
            break;
        case 3:
            difficulty = Difficulty::Hard;
            break;
        case 4:
            difficulty = Difficulty::Custom;
            std::cout << "Custom difficulty selected. Set a point goal: ";
            do {
                try {
                    std::cin >> inputString;

                    inputSetting = std::stoi(inputString);
                    if (inputSetting < 1) {
                        throw std::out_of_range("Out of range: value must be a positive, non-zero integer. ");
                    }
                }
                catch (const std::invalid_argument& e) {
                    std::cerr << "Invalid argument: The string does not begin with a valid number. " << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                } 
                catch (const std::out_of_range& e) {
                    std::cerr << e.what() << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
            } while (inputSetting <= 0);

            SetCustomGoal(inputSetting);
            break;
        default: 
            difficulty = Difficulty::Medium;
    }

    SetDifficulty(difficulty);
    SetScoreGoal(difficulty);
}

void Racko::PlayTurn()
{
    bool isRoundOver = false;
    for (int i = 0; i < playerVector.size() && !isRoundOver; i++) {
        PrintTurnNum();
        
        // score is returned for possible subclasses/accessors, but value is not used in this implementation.
        int playerScore = PlayTurnForPlayerByIdx(i); 

        if (playerScore < 0) {
            std::cerr << ">>> Error in PlayTurnForPlayerByIdx() for player " << i << "\n";
        }
        else if (playerScore > 0) {
            isRoundOver = true;
            for (int j = 0; j < playerVector.size(); j++) {
                ScoreRackByIdx(j);
            }
        }
    }
}

void Racko::PrintTurnNum()
{
    std::cout << ("Round " + std::to_string(numTurns) + "\n");
}

int Racko::CheckForWinners()
{
    for (std::size_t i = 0; i < playerCnt; i++) {
        if (playerVector.at(i)->GetScore() >= score_goal)
            return i;
    }

    return -1;
}

int Racko::FinishGame()
{
    Player* pFirst = nullptr;
    Player* pSecond = nullptr;
    Player* pThird = nullptr;
    Player* pLast = nullptr;
    char playAgainChar;

    for (Player* player : playerVector) {
        int currPlayerScore = player->GetScore();
        if (pFirst == nullptr) {
            player = pFirst;
        }
        else if (pFirst->GetScore() < currPlayerScore) {
            if (pSecond == nullptr) {
                pSecond = pFirst;
                pFirst = player;
            }
            else {
                if (pSecond->GetScore() < currPlayerScore) {
                    pThird = pSecond;
                    pSecond = player;
                }
                else if (pThird) {
                    if (pThird->GetScore() < currPlayerScore) {
                        pThird = player;
                    }
                }
                
            }
        }

        if ( pLast == nullptr || pLast->GetScore() > currPlayerScore) {
            pLast = player;
        }
    }

    std::cout << "--------------------------------------\n";
    std::cout << "First Place: " << (pFirst ? pFirst->GetName() : "") << "\n";
    if (playerCnt > 1) {
        std::cout << "Second Place: " << (pSecond ? pSecond->GetName() : "") << "\n";
        if (playerCnt > 2) {
            std::cout << "Third Place: " << (pThird ? pThird->GetName() : "") << "\n";
            if (playerCnt > 3) {
                std::cout << "Last Place: " << (pLast ? pLast->GetName() : "") << "\n";
            }
        }
    }

    std::cout << "\n\nWould you like to play again? (Y/N) ";
    while (playAgainChar != 'y' && playAgainChar != 'n') {
        std::cin >> playAgainChar;
        if (playAgainChar < 91) {
            playAgainChar += 32;
        }
    }
    if (playAgainChar == 'y') {
        return 1;
    }
    return 0; // Game Over
}

void Racko::ResetGame(bool keepPlayers)
{
    // maybe change delete deck -> add all cards to discardPile, then add all those to deck?
    numTurns = 0;

    deck = std::make_unique<RackoDeck>();
    cardsInDeck = 60;

    while (!discardPile.empty()) {
        discardPile.pop();
    }
    cardsInDiscard = 0;

    if (!keepPlayers) {
        ResetPlayers();
    }
}
