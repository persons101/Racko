#ifndef RACKO_H
#define RACKO_H

#include <stack>
#include <vector>
#include <memory>

#include "card.h"
#include "deck.h"
#include "player.h"
#include "rackoCard.h"

class Racko {
private:
    std::size_t numTurns = 0;
    std::size_t numGames = 0;
    unsigned int playerCnt = 0;
    int cardsInDeck = 60;
    int cardsInDiscard = 0;
    const int RACKO_BONUS_REQ = 50;
    const int RACKO_BONUS = 25;
protected:
    enum class Difficulty {
        Easy,
        Medium,
        Hard,
        Custom
    };
    Difficulty current_difficulty = Difficulty::Medium;
    int custom_difficulty_goal_score = -1;
    int racko_bonus_req_handicap = 0;
    int racko_bonus_handicap = 0;
    int score_goal = 0;
    
    std::unique_ptr<Deck> deck;
    std::stack<Card*> discardPile;
    std::vector<Player*> playerVector;
    
    int GetRackoBonusAmount() const { return RACKO_BONUS; }
    int GetRackoBonusRequirement() const { return RACKO_BONUS_REQ; }
    int GetScoreGoal() const { return score_goal; }
    int GetCustomGoal() const { return custom_difficulty_goal_score; }
    int GetDifficulty() const { return static_cast<int>(current_difficulty); }
    virtual void SetScoreGoal(int);
    virtual void SetScoreGoal(Difficulty);
    virtual void SetDifficulty(Difficulty);
    virtual void SetCustomGoal(int); 
    int GetPlayerIdxByName(std::string playerName) const;

    virtual int CalculateRackScore(const std::vector<Card*>&) const;

    virtual void AddStartingCardsToPlayer(Player*&);
    
    /// @brief Shows the options of card to choose from and lets the player choose.
    /// @param playerIdx Index of the player within playerVector
    /// @return D'r'aw pile, D'i'scard, or 'e'rror
    virtual char SelectDrawCard(int playerIdx) const;
    virtual char SelectDrawCard(Player*) const;
    virtual Card* PopCard(bool);
    virtual int DrawCardForPlayer(int playerIdx, bool);
    virtual int DrawCardForPlayer(Player*, bool);
    virtual int SelectCardIdxToDiscard(int playerIdx, const Card* cardToReplace = nullptr);
    virtual int SelectCardIdxToDiscard(Player*, const Card* cardToReplace = nullptr);
    virtual int AddCardToDiscard(Card*);
    virtual int PlayTurnForPlayerByIdx(int playerIdx);
    virtual int PlayTurnForPlayerByName(std::string playerName);
    virtual void ResetPlayers();
    virtual void AddPlayer(std::string);
    virtual void AddPlayer(Player*);

       
/// @brief Compares a players' score to a currently placed (top 3 / last) player, swapping them if better
/// @param currentlyPlacedPlayer A player that exists in the top 3 / last (e.g. pFirst)
/// @param otherPlayer A player compared to the placement
/// @param arrDescending Used for last placement: places the smaller score in the placement
/// @return Returns the player that does not end in the placement
    Player* PlacementForLarger(Player *&currentlyPlacedPlayer, Player *otherPlayer, bool arrDescending);

public:
    Racko();

    std::size_t GetNumTurns() const;
    std::size_t GetNumGames() const;
    unsigned int GetPlayerCount() const;
    int GetDeckCardCount() const;
    int GetDiscardCardCount() const;
    
    Card* GetTopCardFromDeck() const;
    Card* GetTopCardFromDiscard() const;
    std::vector<Card*> GetPlayerCardsByIdx(int playerIdx) const;
    std::vector<Card*> GetPlayerCardsByName(std::string playerName) const;
    Player* GetPlayerByIdx(int playerIdx) const;
    Player* GetPlayerByName(std::string playerName) const;
    
    void ScoreRackByIdx(int playerIdx);
    void ScoreRackByName(std::string playerName);
    
    virtual void PrintTurnNum();
    virtual void PrintPlayerDetails(Player*);

    virtual void SetupGame();
    virtual Player* CreatePlayer();
    virtual void ChooseDifficulty();
    virtual int Run();
    virtual void PlayTurn();
    virtual int CheckForWinners();
    virtual int FinishGame();
    virtual void ResetGame(bool keepPlayers = false);
    
};


#endif