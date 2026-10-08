#include "catch_amalgamated.hpp"

#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include "../src/card.h"
#include "../src/Deck.h"
#include "../src/racko.h"
#include "../src/player.h"
#include "../src/rackoCard.h"
#include "../src/rackoDeck.h"
#include "src/testDeck.h"
#include "src/testPlayer.h"
#include "src/testRacko.h"

class ScopedStreamRedirect {
public:
    ScopedStreamRedirect(std::istream& replacementInput, std::ostream& replacementOutput)
        : oldInput(std::cin.rdbuf(replacementInput.rdbuf())),
          oldOutput(std::cout.rdbuf(replacementOutput.rdbuf())) {}

    ~ScopedStreamRedirect() {
        std::cin.rdbuf(oldInput);
        std::cout.rdbuf(oldOutput);
    }

private:
    std::streambuf* oldInput;
    std::streambuf* oldOutput;
};


TEST_CASE("Testing Suit to string", "[Suit],[suit_name]"){
    REQUIRE(Card::suit_name(Suit::SPADES) == "Spades");
    REQUIRE(Card::suit_name(Suit::HEARTS) == "Hearts");
    REQUIRE(Card::suit_name(Suit::DIAMONDS) == "Diamonds");
    REQUIRE(Card::suit_name(Suit::CLUBS) == "Clubs");
}

TEST_CASE("Testing Suit to int", "[Suit]"){
    REQUIRE(static_cast<int>(Suit::SPADES) == 1);
    REQUIRE(static_cast<int>(Suit::HEARTS) == 2);
    REQUIRE(static_cast<int>(Suit::DIAMONDS) == 3);
    REQUIRE(static_cast<int>(Suit::CLUBS) == 4);
}

TEST_CASE("Card methods", "[Card]") {
    std::vector<Card*> cards;
    Card* cardJokerHeart = new Card(0, Suit::HEARTS);
    REQUIRE(cardJokerHeart);
    cards.push_back(cardJokerHeart);
    Card* aceOfSpades = new Card(1, Suit::SPADES);
    REQUIRE(aceOfSpades);
    cards.push_back(aceOfSpades);
    for (int i = 2; i <= 10; i++) {
        cards.push_back(new Card(i, Suit(i % 4 + 1)));
    }
    Card* sevenOfClubs = cards.at(7);
    REQUIRE(sevenOfClubs->getSuit() == Suit::CLUBS);
    REQUIRE(sevenOfClubs->getValue() == 7);

    std::tuple<int, Suit> cardTuple = std::tuple(11, Suit::DIAMONDS);
    Card* jackOfDiamonds = new Card(cardTuple);
    REQUIRE(jackOfDiamonds);
    cards.push_back(jackOfDiamonds);
    for (int i = 12; i <= 14; i++) {
        cards.push_back(new Card(i, Suit::DIAMONDS));
    }

    for (std::size_t i = 0; i < cards.size(); i++) {
        REQUIRE(cards.at(i)->getValue() == i);
    }

    SECTION("Card methods - getValueChar", "[Card::getValueChar]") {
        REQUIRE(cards.at(0)->getValueChar() == 'X');
        REQUIRE(aceOfSpades->getValueChar() == 'A');
        REQUIRE(cards.at(5)->getValueChar() == '5');
        REQUIRE(jackOfDiamonds->getValueChar() == 'J');
        REQUIRE(cards.at(12)->getValueChar() == 'Q');
        REQUIRE(cards.at(13)->getValueChar() == 'K');
        REQUIRE(cards.at(14)->getValueChar() == '^');
        REQUIRE(Card(-1,Suit::CLUBS).getValueChar() == '_');
    }

    SECTION("Card methods - getSuit[name]", "[Card::getSuit][Card::getSuitName]") {
        REQUIRE(cardJokerHeart->getSuit() == Suit::HEARTS);
        REQUIRE(cardJokerHeart->getSuitName() == "Hearts");
        REQUIRE(cardJokerHeart->getSuitSymbol() == "\xe2\x99\xA5");
        REQUIRE(aceOfSpades->getSuit() == Suit::SPADES);
        REQUIRE(aceOfSpades->getSuitName() == "Spades"); 
        REQUIRE(aceOfSpades->getSuitSymbol() == "\xe2\x99\xA0");
        REQUIRE(sevenOfClubs->getSuit() == Suit::CLUBS);
        REQUIRE(sevenOfClubs->getSuitName() == "Clubs");
        REQUIRE(sevenOfClubs->getSuitSymbol() == "\xe2\x99\xA3");
        REQUIRE(jackOfDiamonds->getSuit() == Suit::DIAMONDS);
        REQUIRE(jackOfDiamonds->getSuitName() == "Diamonds"); 
        REQUIRE(jackOfDiamonds->getSuitSymbol() == "\xe2\x99\xA6");
    }
}

TEST_CASE("Game class creation", "[Game],[constructor]"){
    Racko game;
}

TEST_CASE_METHOD(Player, "Player class creation", "[Player],[constructor]"){
    SECTION("Player score management"){
        REQUIRE(GetScore() == 0);
        REQUIRE(AddScore(25) == 25);
        REQUIRE(GetScore() == 25);
    }
    SECTION("Player card management"){
        REQUIRE(GetCards().empty());
        
        REQUIRE(DrawCard(new Card(1, Suit::HEARTS)));

        REQUIRE(GetCards().size() == 1);

        for (int i = 2; i <= 10; i++){
            REQUIRE(DrawCard(new Card(i, Suit::HEARTS)));
        }

        REQUIRE(GetCards().size() == 10);

        Card* discardedCard = DiscardCard(5, Suit::HEARTS);

        REQUIRE(discardedCard != nullptr);

        REQUIRE(GetCards().size() == 9);

        discardedCard = DiscardRandomCard();

        REQUIRE(discardedCard != nullptr);
        REQUIRE(discardedCard->getSuit() == HEARTS);
        REQUIRE(GetCards().size() == 8);
    }

    SECTION("Full deck card management"){
        Player player1("Steve");

        for (int suit = 1; suit <= 4; suit++)
            for (int cardVal = 1; cardVal <= 13; cardVal++)
                player1.DrawCard(new Card(cardVal, (Suit)suit));

        REQUIRE(player1.GetCards().size() == 13*4);

        for (int suit = 1; suit <= 4; suit++)
            for (int cardVal = 1; cardVal <= 13; cardVal++)
                player1.DiscardRandomCard();

        REQUIRE(player1.GetCards().size() == 0);
    }
}

TEST_CASE("Player drawing from deck", "[Player][Player::DrawCard][Deck][Deck::PopTopCard]"){
    Deck deck(60,1);
    Player player1("Steve");

    REQUIRE(deck.GetNumCards() == 60);
    REQUIRE(player1.GetCards().size() == 0);

    for (int suit = 1; suit <= 1; suit++)
        for (int cardVal = 1; cardVal <= 60; cardVal++)
            player1.DrawCard(deck.PopTopCard());

    REQUIRE(deck.GetNumCards() == 0);
    REQUIRE(player1.GetCards().size() == 60);
}

TEST_CASE("Trying RackoDeck", "[RackoDeck]"){
    RackoDeck deck;

    REQUIRE(deck.GetNumCards() == 60);

    Card* topCard = deck.PopTopCard();
    REQUIRE(topCard != nullptr);
    REQUIRE( ( (topCard->getSuit()) == (Suit)0) );
}

TEST_CASE("RackoCard vs Card", "[RackoCard][cout]")
{
    /// GOAL: Create RackoCards with values 1, 11, 12, and 13, (usually A, J, Q, K), and confirm they print numbers and not letters. This ensures the inheritance is working

    std::stringstream oss;

    // Execute code that writes to cout
    auto cardValue = GENERATE(1, 11, 12, 13);

    RackoCard rackoCard = RackoCard(cardValue);
    oss << rackoCard.PrintCardShort();


    // Check the captured output
    REQUIRE(oss.str() == (std::to_string(rackoCard.getValue()) ) );
}

TEST_CASE_METHOD(TestRacko, "TestPlayer functions", "[TestPlayer][Racko]") {
    TestPlayer p1;
    std::vector<Card*> cards = p1.GetCards(); // 1,5,10,15,20,25,30,35,36,38,40,2,3,4,6 
    REQUIRE(CalculateRackScore(cards) == 11 * 5 + GetRackoBonusAmount());
    REQUIRE(p1.AddScore(CalculateRackScore(cards)) == 55 + GetRackoBonusAmount());
    REQUIRE(p1.GetScore() == 55 + GetRackoBonusAmount());

}

TEST_CASE_METHOD(TestRacko, "Racko game initialization", "[Racko]") {
    REQUIRE(GetNumTurns() == 0);
    REQUIRE(GetDeckCardCount() == 60);
    REQUIRE(GetDiscardCardCount() == 0);
    REQUIRE(GetPlayerCount() == 0);
}

TEST_CASE_METHOD(TestRacko, "Racko game score calculation", "[Racko][RackoCard][Score][Player]") {
    // https://www.hasbro.com/common/instruct/Racko%281987%29.PDF
    RackoCard* card1 = new RackoCard(1);
    RackoCard* card2 = new RackoCard(2);
    RackoCard* card3 = new RackoCard(3);
    RackoCard* card4 = new RackoCard(4);
    RackoCard* card5 = new RackoCard(5);
    RackoCard* card6 = new RackoCard(6);

    SECTION("Check Calculations") {
        std::vector<Card*> oneCard = {card6, card2, card3, card5, card4, card1};
        std::vector<Card*> fourCards = {card1, card2, card3, card5, card4, card6};
        std::vector<Card*> fiveCards = {card1, card2, card3, card4, card6, card5};

        REQUIRE(CalculateRackScore(fourCards) == (4 * 5)); 
        REQUIRE(CalculateRackScore(fiveCards) == (5 * 5));

    }

    SECTION("Add Player by name") {
        Player p1("Steve");
        AddPlayer("Steve");
        REQUIRE(GetPlayerCount() == 1);
        REQUIRE(p1.GetName() == GetPlayerByIdx(0)->GetName());
    }

    SECTION("Add Player by reference") {
        Player p1("Steve");
        TestPlayer* p2 = new TestPlayer("Steve");
        AddPlayer(&p1);
        AddPlayer(p2);
        REQUIRE(GetPlayerCount() == 2);

        REQUIRE(p1.GetName() == GetPlayerByIdx(1)->GetName());
    }

    SECTION("One player turn") {
        TestPlayer* p1 = new TestPlayer("Steve"); // {1,5,10,18,20,25,30,35,36,38,40,2,3,4,6 } 
        AddPlayer(p1);
        REQUIRE(GetPlayerCount() == 1);

        REQUIRE(p1->GetName() == GetPlayerByIdx(0)->GetName());

        deck = std::make_unique<TestDeck>();// top on left: 11,12,13,14,15

        std::istringstream input("0\n5\n");
        std::ostringstream output;
        
        auto* oldInput = std::cin.rdbuf(input.rdbuf());
        auto* oldOutput = std::cout.rdbuf(output.rdbuf());
        
        PlayTurnForPlayerByIdx(0);

        std::cin.rdbuf(oldInput);
        std::cout.rdbuf(oldOutput);


        REQUIRE(GetPlayerByIdx(0)->GetCardVals() == std::vector({11,5,10,18,20,25,30,35,36,38,40,2,3,4,6 }));
    }

    delete card1;
    delete card2;
    delete card3;
    delete card4;
    delete card5;
    delete card6;
}


//  Racko::SelectDrawCard() -> 
//  - test when discardPile is empty
//  - check newest card drawn is correct card
//  - test garbage characters
//  - test long strings
//  = test expected behaviors

TEST_CASE_METHOD(TestRacko, "SelectDrawCard returns error for invalid player",
                 "[Racko][SelectDrawCard]") {
    REQUIRE(SelectDrawCard(-1) == 'e');
}

TEST_CASE_METHOD(TestRacko, "SelectDrawCard chooses the deck",
                 "[Racko][SelectDrawCard]") {
    AddPlayer("Steve");

    std::istringstream input("0\n");
    std::ostringstream output;

    auto* oldInput = std::cin.rdbuf(input.rdbuf());
    auto* oldOutput = std::cout.rdbuf(output.rdbuf());

    char result = SelectDrawCard(0);

    std::cin.rdbuf(oldInput);
    std::cout.rdbuf(oldOutput);

    REQUIRE(result == 'r');
    REQUIRE(output.str().find("Choose a card") != std::string::npos);
}

TEST_CASE_METHOD(TestRacko, "SelectDrawCard chooses the discard pile",
                 "[Racko][SelectDrawCard]") {
    AddPlayer("Steve");
    discardPile.push(new Card(42, Suit::HEARTS));

    std::istringstream input("1\n");
    std::ostringstream output;

    auto* oldInput = std::cin.rdbuf(input.rdbuf());
    auto* oldOutput = std::cout.rdbuf(output.rdbuf());

    char result = SelectDrawCard(0);

    std::cin.rdbuf(oldInput);
    std::cout.rdbuf(oldOutput);

    REQUIRE(result == 'i');
    REQUIRE(output.str().find("Discard") != std::string::npos);
}

TEST_CASE_METHOD(TestRacko, "SelectDrawCard rejects invalid input before deck choice",
                 "[Racko][SelectDrawCard]") {
    AddPlayer("Steve");

    std::istringstream input("x\n0\n");
    std::ostringstream output;

    auto* oldInput = std::cin.rdbuf(input.rdbuf());
    auto* oldOutput = std::cout.rdbuf(output.rdbuf());

    char result = SelectDrawCard(0);

    std::cin.rdbuf(oldInput);
    std::cout.rdbuf(oldOutput);

    REQUIRE(result == 'r');
}

TEST_CASE_METHOD(TestRacko, "SelectDrawCard rejects discard choice when pile is empty",
                 "[Racko][SelectDrawCard]") {
    AddPlayer("Steve");

    std::istringstream input("1\n0\n");
    std::ostringstream output;

    auto* oldInput = std::cin.rdbuf(input.rdbuf());
    auto* oldOutput = std::cout.rdbuf(output.rdbuf());

    char result = SelectDrawCard(0);

    std::cin.rdbuf(oldInput);
    std::cout.rdbuf(oldOutput);

    REQUIRE(result == 'r');
}

TEST_CASE_METHOD(TestRacko, "SelectDrawCard accepts discard choice when pile is not empty",
                 "[Racko][SelectDrawCard]") {
    AddPlayer("Steve");
    discardPile.push(new Card(25, Suit::CLUBS));

    std::istringstream input("1\n");
    std::ostringstream output;

    auto* oldInput = std::cin.rdbuf(input.rdbuf());
    auto* oldOutput = std::cout.rdbuf(output.rdbuf());

    char result = SelectDrawCard(0);

    std::cin.rdbuf(oldInput);
    std::cout.rdbuf(oldOutput);

    REQUIRE(result == 'i');
    REQUIRE(output.str().find("25") != std::string::npos);
}

TEST_CASE_METHOD(TestRacko, "SelectDrawCard rejects multi-character choices",
                 "[Racko][SelectDrawCard]") {
    AddPlayer("Steve");

    std::istringstream input("10\n0\n");
    std::ostringstream output;

    auto* oldInput = std::cin.rdbuf(input.rdbuf());
    auto* oldOutput = std::cout.rdbuf(output.rdbuf());

    char result = SelectDrawCard(0);

    std::cin.rdbuf(oldInput);
    std::cout.rdbuf(oldOutput);

    REQUIRE(result == 'r');
}

TEST_CASE("Card accessors, formatting, and comparisons", "[Card]") {
    Card joker(0, Suit::HEARTS);
    Card aceOfSpades(1, Suit::SPADES);
    Card kingOfSpades(13, Suit::SPADES);
    Card invalidValue(-1, Suit::CLUBS);

    REQUIRE(joker.getValue() == 0);
    REQUIRE(joker.getSuit() == Suit::HEARTS);
    REQUIRE(joker.getValueString() == "Joker");
    REQUIRE(joker.getSuitName() == "Hearts");
    REQUIRE(joker.getSuitSymbol() == "\xe2\x99\xa5");
    REQUIRE(joker.isBlack() == false);
    REQUIRE(aceOfSpades.isBlack() == true);
    REQUIRE(kingOfSpades.PrintCard() == "Card Value: 13, Suit: Spades");
    REQUIRE(aceOfSpades.PrintCardShort() == "A\xe2\x99\xa0");
    REQUIRE(invalidValue.getValueString() == "-1");

    Card sameAce(1, Suit::SPADES);
    Card differentAce(1, Suit::HEARTS);
    REQUIRE(aceOfSpades == sameAce);
    REQUIRE_FALSE(aceOfSpades != sameAce);
    REQUIRE(aceOfSpades != differentAce);
    REQUIRE_FALSE(aceOfSpades == differentAce);

    Card::compareCards compare;
    REQUIRE(compare(Card(1, Suit::SPADES), Card(2, Suit::SPADES)));
    REQUIRE(compare(Card(1, Suit::SPADES), Card(1, Suit::HEARTS)));
    REQUIRE(compare(Card(1, Suit::SPADES), std::make_tuple(2, Suit::SPADES)));
    REQUIRE_FALSE(compare(Card(2, Suit::SPADES), Card(1, Suit::SPADES)));
}

TEST_CASE("Deck accessors, boundaries, shuffle, and output", "[Deck]") {
    Deck deck(2, 2, 1);

    REQUIRE(deck.GetNumCards() == 5);
    REQUIRE(deck.GetCards().size() == 5);
    REQUIRE(*deck.GetTopCard() == std::make_tuple(1, Suit::SPADES));
    REQUIRE(*deck.GetBottomCard() == std::make_tuple(0, Suit::SPADES));

    Card* topCard = deck.PopTopCard();
    Card* bottomCard = deck.PopBottomCard();
    REQUIRE(*topCard == std::make_tuple(1, Suit::SPADES));
    REQUIRE(*bottomCard == std::make_tuple(0, Suit::SPADES));
    delete topCard;
    delete bottomCard;
    REQUIRE(deck.GetNumCards() == 3);

    auto cardsBeforeShuffle = deck.GetCards();
    deck.Shuffle();
    REQUIRE(deck.GetNumCards() == 3);
    REQUIRE(deck.GetCards().size() == cardsBeforeShuffle.size());

    std::ostringstream output;
    output << deck.PrintDeck();
    REQUIRE(output.str().find("---Start Deck---") != std::string::npos);
    REQUIRE(output.str().find("--- End Deck ---") != std::string::npos);

    Deck emptyDeck(0, 0);
    REQUIRE(emptyDeck.GetNumCards() == 0);
    REQUIRE(emptyDeck.GetCards().empty());
    REQUIRE(emptyDeck.GetTopCard() == nullptr);
    REQUIRE(emptyDeck.GetBottomCard() == nullptr);
    REQUIRE(emptyDeck.PopTopCard() == nullptr);
    REQUIRE(emptyDeck.PopBottomCard() == nullptr);
}

TEST_CASE_METHOD(TestRacko, "Racko difficulty and score goal settings",
                 "[Racko][Difficulty][ScoreGoal]") {
    SECTION("Direct score goals include boundaries and negative values") {
        SetScoreGoal(0);
        REQUIRE(GetScoreGoal() == 0);

        SetScoreGoal(150);
        REQUIRE(GetScoreGoal() == 150);

        SetScoreGoal(-1);
        REQUIRE(GetScoreGoal() == -1);
    }

    SECTION("Difficulty state and custom goal are stored") {
        SetCustomGoal(1);
        REQUIRE(GetCustomGoal() == 1);
        SetDifficulty(Difficulty::Easy);
        REQUIRE(GetDifficulty() == 0);
        SetDifficulty(Difficulty::Custom);
        REQUIRE(GetDifficulty() == 3);
    }

    SECTION("Preset difficulties select their documented score goals") {
        SetScoreGoal(Difficulty::Easy);
        REQUIRE(GetScoreGoal() == 100);

        SetScoreGoal(Difficulty::Medium);
        REQUIRE(GetScoreGoal() == 150);

        SetScoreGoal(Difficulty::Hard);
        REQUIRE(GetScoreGoal() == 200);

        SetCustomGoal(75);
        SetScoreGoal(Difficulty::Custom);
        REQUIRE(GetScoreGoal() == 75);
    }
}

TEST_CASE_METHOD(TestRacko, "Racko difficulty selection handles invalid input",
                 "[Racko][Difficulty][ScoreGoal]") {
    std::istringstream input("x\n0\n2\n");
    auto* oldInput = std::cin.rdbuf(input.rdbuf());

    ChooseDifficulty();

    std::cin.rdbuf(oldInput);
    REQUIRE(GetDifficulty() == 1);
    REQUIRE(GetScoreGoal() == 150);
}

TEST_CASE("RackoPlayer construction and center formatting", "[RackoPlayer]") {
    TestPlayer player("Alex");
    TestPlayer emptyName("");

    REQUIRE(player.GetName() == "Alex");
    REQUIRE(emptyName.GetName().empty());
    REQUIRE(player.GetNumCards() == 15);
    REQUIRE(emptyName.GetNumCards() == 15);
    REQUIRE(player.GetScore() == 0);

    REQUIRE(player.center("x", 5) == "  x  ");
    REQUIRE(player.center("x", 4, '.') == ".x..");
    REQUIRE(player.center("text", 4) == "text");
    REQUIRE(player.center("long text", 4) == "long text");
}

TEST_CASE("RackoPlayer displays rack values and positions", "[RackoPlayer]") {
    TestPlayer player("Alex");
    player.ResetPlayer();

    REQUIRE(player.PrintCards() == "Card: \nIdx:  ");

    player.DrawCard(new Card(1, Suit::SPADES));
    player.DrawCard(new Card(10, Suit::HEARTS));
    player.DrawCard(new Card(13, Suit::CLUBS));

    const std::string display = player.PrintCards();
    REQUIRE(display.find(" 1  ") != std::string::npos);
    REQUIRE(display.find(" 10 ") != std::string::npos);
    REQUIRE(display.find(" 13 ") != std::string::npos);
    REQUIRE(display.find(" 15 ") != std::string::npos);
    REQUIRE(display.find(" 5  ") != std::string::npos); 
    REQUIRE(display.find("10 ") != std::string::npos);
    REQUIRE(display.find('\n') != std::string::npos);
}

TEST_CASE("RackoPlayer completion checks rack ordering", "[RackoPlayer]") {
    TestPlayer oneCardPlayer("One");
    oneCardPlayer.ResetPlayer();
    oneCardPlayer.DrawCard(new Card(7, Suit::SPADES));
    REQUIRE(oneCardPlayer.IsCompletedWithRack());

    TestPlayer increasingPlayer("Increasing");
    increasingPlayer.ResetPlayer();
    increasingPlayer.DrawCard(new Card(1, Suit::SPADES));
    increasingPlayer.DrawCard(new Card(2, Suit::SPADES));
    increasingPlayer.DrawCard(new Card(9, Suit::SPADES));
    REQUIRE(increasingPlayer.IsCompletedWithRack());

    TestPlayer duplicatePlayer("Duplicate");
    duplicatePlayer.ResetPlayer();
    duplicatePlayer.DrawCard(new Card(1, Suit::SPADES));
    duplicatePlayer.DrawCard(new Card(1, Suit::HEARTS));
    REQUIRE_FALSE(duplicatePlayer.IsCompletedWithRack());

    TestPlayer descendingPlayer("Descending");
    descendingPlayer.ResetPlayer();
    descendingPlayer.DrawCard(new Card(9, Suit::SPADES));
    descendingPlayer.DrawCard(new Card(2, Suit::SPADES));
    REQUIRE_FALSE(descendingPlayer.IsCompletedWithRack());
}

TEST_CASE("RackoPlayer replaces cards at valid and invalid positions", "[RackoPlayer]") {
    TestPlayer player("Alex");
    player.ResetPlayer();
    player.DrawCard(new Card(1, Suit::SPADES));
    player.DrawCard(new Card(2, Suit::HEARTS));
    player.DrawCard(new Card(3, Suit::CLUBS));

    Card* oldFirst = player.ReplaceCard(new Card(10, Suit::DIAMONDS), 0);
    REQUIRE(oldFirst != nullptr);
    REQUIRE(oldFirst->getValue() == 1);
    delete oldFirst;
    REQUIRE(player.GetCards().at(0)->getValue() == 10);

    Card* oldLast = player.ReplaceCard(new Card(20, Suit::DIAMONDS), 2);
    REQUIRE(oldLast != nullptr);
    REQUIRE(oldLast->getValue() == 3);
    delete oldLast;
    REQUIRE(player.GetCards().at(2)->getValue() == 20);

    const std::vector<int> valuesBeforeInvalid = player.GetCardVals();
    REQUIRE(player.ReplaceCard(nullptr, 1) == nullptr);
    Card* negativePositionCard = new Card(30, Suit::DIAMONDS);
    Card* outOfBoundsCard = new Card(30, Suit::DIAMONDS);
    REQUIRE(player.ReplaceCard(negativePositionCard, -1) == nullptr);
    REQUIRE(player.ReplaceCard(outOfBoundsCard, 3) == nullptr);
    delete negativePositionCard;
    delete outOfBoundsCard;
    REQUIRE(player.GetCardVals() == valuesBeforeInvalid);
}

TEST_CASE("RackoPlayer discards valid and invalid positions", "[RackoPlayer]") {
    TestPlayer player("Alex");
    player.ClearCards();
    player.DrawCard(new Card(1, Suit::SPADES));
    player.DrawCard(new Card(2, Suit::HEARTS));
    player.DrawCard(new Card(3, Suit::CLUBS));

    Card* first = player.DiscardCard(0);
    REQUIRE(first != nullptr);
    REQUIRE(first->getValue() == 1);
    delete first;
    REQUIRE(player.GetNumCards() == 2);

    Card* last = player.DiscardCard(1, Suit::DIAMONDS);
    REQUIRE(last != nullptr);
    REQUIRE(last->getValue() == 3);
    delete last;
    REQUIRE(player.GetNumCards() == 1);

    REQUIRE(player.DiscardCard(-1) == nullptr);
    REQUIRE(player.DiscardCard(1) == nullptr);
    REQUIRE(player.GetNumCards() == 1);
}

TEST_CASE("RackoPlayer selects a replacement card after invalid input", "[RackoPlayer]") {
    TestPlayer player("Alex");
    player.ResetPlayer();
    player.DrawCard(new Card(1, Suit::SPADES));
    player.DrawCard(new Card(10, Suit::HEARTS));
    player.DrawCard(new Card(15, Suit::CLUBS));

    std::istringstream input("x\n0\n999999999999999999999\n15\n");
    std::ostringstream output;
    auto* oldInput = std::cin.rdbuf(input.rdbuf());
    auto* oldOutput = std::cout.rdbuf(output.rdbuf());

    const int selected = player.SelectCardToReplace(new Card(35, Suit::DIAMONDS));

    std::cin.rdbuf(oldInput);
    std::cout.rdbuf(oldOutput);

    REQUIRE(selected == 2);
    REQUIRE(output.str().find("Choose a card to remove") != std::string::npos);
}

TEST_CASE("Player reset and ordered value access", "[Player][ResetPlayer][GetCardVals][PrintCards]") {
    Player player("Steve");

    REQUIRE(player.GetCardVals().empty());
    REQUIRE(player.PrintCards() == "Steve's hand:");

    player.DrawCard(new Card(7, Suit::HEARTS));
    player.DrawCard(new Card(3, Suit::CLUBS));
    player.DrawCard(new Card(9, Suit::SPADES));

    REQUIRE(player.GetCardVals() == std::vector<int>({7, 3, 9}));
    REQUIRE(player.GetNumCards() == 3);
    REQUIRE(player.PrintCards() == "Steve's hand: 7♥ 3♣ 9♠");

    const std::vector<Card*> returnedCards = player.ResetPlayer(true);
    REQUIRE(returnedCards.size() == 3);
    REQUIRE(returnedCards.at(0)->getValue() == 7);
    REQUIRE(returnedCards.at(1)->getValue() == 3);
    REQUIRE(returnedCards.at(2)->getValue() == 9);
    REQUIRE(player.GetCards().empty());
    REQUIRE(player.GetNumCards() == 0);
    REQUIRE(player.GetScore() == 0);
    REQUIRE(player.PrintCards() == "Steve's hand:");

    for (Card* card : returnedCards) {
        delete card;
    }
}

TEST_CASE_METHOD(TestRacko, "Racko exposes valid deck, discard, and player lookup values",
                 "[Racko][Lookup][Deck][Discard]") {
    deck = std::make_unique<RackoDeck>();
    REQUIRE(GetTopCardFromDeck() != nullptr);
    REQUIRE(deck->GetCards().size() == 60);
    REQUIRE(GetTopCardFromDiscard() == nullptr);

    discardPile.push(new Card(42, Suit::HEARTS));
    REQUIRE(GetTopCardFromDiscard() != nullptr);
    REQUIRE(GetTopCardFromDiscard()->getValue() == 42);
    AddCardToDiscard(new Card(35, Suit::SPADES));
    REQUIRE(GetTopCardFromDiscard() != nullptr);
    REQUIRE(GetTopCardFromDiscard()->getValue() == 35);

    TestPlayer* alpha = new TestPlayer("Alpha");
    TestPlayer* bravo = new TestPlayer("Bravo");
    alpha->ResetPlayer(false);
    bravo->ResetPlayer(false);
    alpha->DrawCard(new Card(7, Suit::SPADES));
    alpha->DrawCard(new Card(11, Suit::HEARTS));
    bravo->DrawCard(new Card(4, Suit::CLUBS));

    AddPlayer(alpha);
    AddPlayer(bravo);

    std::vector<Card*> alphaCards = GetPlayerCardsByIdx(0);
    REQUIRE(alphaCards.size() == 2);
    REQUIRE(alphaCards.at(0)->getValue() == 7);
    REQUIRE(alphaCards.at(1)->getValue() == 11);

    std::vector<Card*> bravoCards = GetPlayerCardsByName("Bravo");
    REQUIRE(bravoCards.size() == 1);
    REQUIRE(bravoCards.at(0)->getValue() == 4);

    REQUIRE(GetPlayerByIdx(1) == bravo);
    REQUIRE(GetPlayerByName("Alpha") == alpha);
    REQUIRE(GetPlayerByIdx(-1) == nullptr);
    REQUIRE(GetPlayerByName("Ghost") == nullptr);
}

TEST_CASE_METHOD(TestRacko, "Racko scores valid player hands by index and name",
                 "[Racko][ScoreRack][Player]") {
    TestPlayer* alpha = new TestPlayer("Alpha");
    TestPlayer* bravo = new TestPlayer("Bravo");

    alpha->ResetPlayer(false);
    alpha->SetCards({1,2,3,4,5,6,7,8,9,10});
    bravo->ResetPlayer(false);
    bravo->SetCards({1,4,6,9,14,16,17,48,49,52});

    AddPlayer(alpha);
    AddPlayer(bravo);

    REQUIRE(alpha->GetScore() == 0);
    ScoreRackByIdx(0);
    REQUIRE(alpha->GetScore() == (50 + GetRackoBonusAmount()));

    REQUIRE(bravo->GetScore() == 0);
    ScoreRackByName("Bravo");
    REQUIRE(bravo->GetScore() == (50 + GetRackoBonusAmount()));
}

TEST_CASE_METHOD(TestPlayer, "RackoPlayer centers text numbers", "[RackoPlayer][center]") {
    std::string twoChar = "ab";
    std::string threeChar = "abc";
    std::string fourChar = "abcd";
    REQUIRE(center(twoChar, 4) == " ab ");
    REQUIRE(center(threeChar, 4) == "abc ");
    REQUIRE(center(fourChar, 4) == "abcd");
}

TEST_CASE_METHOD(TestRacko, "Racko creates a player from a name", "[Racko][CreatePlayer]") {
    std::string name = "Kathy";
    
    std::istringstream input(name + "\n1");
    std::ostringstream output;
    auto* oldInput = std::cin.rdbuf(input.rdbuf());
    auto* oldOutput = std::cout.rdbuf(output.rdbuf());
    
    Player* player = CreatePlayer();

    std::cin.rdbuf(oldInput);
    std::cout.rdbuf(oldOutput);

    REQUIRE(player->GetName() == name);

    
}

TEST_CASE_METHOD(TestRacko, "Racko CreatePlayer handles whitespace and end of input",
                 "[Racko][CreatePlayer]") {
    SECTION("Leading whitespace is skipped before reading the name") {
        std::istringstream input(" \t\n  Kathy\n");
        std::ostringstream output;
        Player* createdPlayer = nullptr;
        {
            ScopedStreamRedirect redirect(input, output);
            createdPlayer = CreatePlayer();
        }

        std::unique_ptr<Player> playerOwner(createdPlayer);
        REQUIRE(playerOwner != nullptr);
        REQUIRE(playerOwner->GetName() == "Kathy");
        REQUIRE(dynamic_cast<RackoPlayer*>(playerOwner.get()) != nullptr);
    }

    SECTION("End of input returns nullptr") {
        std::istringstream input;
        std::ostringstream output;
        Player* player = nullptr;
        {
            ScopedStreamRedirect redirect(input, output);
            player = CreatePlayer();
        }

        REQUIRE(player == nullptr);
    }
}

TEST_CASE("Player keeps numCards synchronized", "[Player][numCards][GetNumCards]") {
    TestPlayer player = TestPlayer();
    player.ResetPlayer();
    REQUIRE(player.GetNumCards() == player.GetCards().size());

    player.DrawCard(new Card(1, Suit::SPADES));
    REQUIRE(player.GetNumCards() == player.GetCards().size());

    player.DrawCard(new Card(2, Suit::HEARTS));
    REQUIRE(player.GetNumCards() == player.GetCards().size());

    player.DrawCard(new Card(3, Suit::CLUBS));
    REQUIRE(player.GetNumCards() == player.GetCards().size());

    Card* replacedCard = (player.ReplaceCard(new Card(4, Suit::DIAMONDS), 0));
    REQUIRE(player.GetNumCards() == player.GetCards().size());
    REQUIRE(replacedCard->getValue() == 1);
    REQUIRE(player.GetCardVals() == std::vector<int>({4,2,3}));

    Card* replacedCard2 = player.DiscardCard(0);
    REQUIRE(player.GetNumCards() == player.GetCards().size());
    REQUIRE(replacedCard2->getValue() == 4);    

    Card* replacedCard3 = player.DiscardRandomCard();
    REQUIRE(player.GetNumCards() == player.GetCards().size());

}

TEST_CASE_METHOD(TestRacko, "Racko draws cards from the deck and discard pile",
                 "[Racko][DrawCardForPlayer]") {
    TestPlayer player("Draw");
    player.ClearCards();
    deck = std::make_unique<TestDeck>();

    REQUIRE(DrawCardForPlayer(nullptr, false) == -1);
    REQUIRE(DrawCardForPlayer(&player, false) == 0);
    REQUIRE(player.GetCardVals() == std::vector<int>{11});
    REQUIRE(GetDeckCardCount() == 59);

    AddCardToDiscard(new Card(42, Suit::HEARTS));
    REQUIRE(DrawCardForPlayer(&player, true) == 0);
    REQUIRE(player.GetCardVals() == std::vector<int>{11, 42});
    REQUIRE(GetDiscardCardCount() == 0);

    REQUIRE(DrawCardForPlayer(&player, true) == 1);
    REQUIRE(player.GetNumCards() == 2);

    auto* indexedPlayer = new TestPlayer("Indexed");
    indexedPlayer->ClearCards();
    AddPlayer(indexedPlayer);
    deck = std::make_unique<TestDeck>();

    REQUIRE(DrawCardForPlayer(-1, false) == -1);
    REQUIRE(DrawCardForPlayer(0, false) == 0);
    REQUIRE(indexedPlayer->GetCardVals() == std::vector<int>{11});
    REQUIRE(GetDeckCardCount() == 59);
    ResetPlayers();
}

TEST_CASE_METHOD(TestRacko, "Racko finds player indexes by name",
                 "[Racko][GetPlayerIdxByName]") {
    AddPlayer("Alpha");
    AddPlayer("Bravo");
    AddPlayer("Alpha");
    AddPlayer("");

    REQUIRE(GetPlayerIdxByName("Alpha") == 0);
    REQUIRE(GetPlayerIdxByName("Bravo") == 1);
    REQUIRE(GetPlayerIdxByName("") == 3);

    ResetPlayers();
}

TEST_CASE_METHOD(TestRacko, "Racko resets the player collection",
                 "[Racko][ResetPlayers]") {
    AddPlayer("Alpha");
    AddPlayer("Bravo");
    REQUIRE(GetPlayerCount() == 2);

    ResetPlayers();
    REQUIRE(GetPlayerCount() == 0);

    ResetPlayers();
    REQUIRE(GetPlayerCount() == 0);
}

TEST_CASE_METHOD(TestRacko, "Racko reports an empty deck during a named player turn",
                 "[Racko][PlayTurnForPlayerByName]") {
    auto* player = new TestPlayer("Alpha");
    player->SetCards(std::vector<int>{1, 5, 10});
    AddPlayer(player);
    deck = std::make_unique<Deck>(0, 0);

    const int result = PlayTurnForPlayerByName("Alpha");

    REQUIRE(result == -3);
    REQUIRE(player->GetCardVals() == std::vector<int>{1, 5, 10});
}

TEST_CASE_METHOD(TestRacko, "Racko resets game state and optionally keeps players",
                 "[Racko][ResetGame]") {
    PlayTurn();
    REQUIRE(GetNumTurns() == 1);

    AddPlayer("Kept");
    Card discardedCard(42, Suit::HEARTS);
    AddCardToDiscard(&discardedCard);

    ResetGame(true);
    REQUIRE(GetNumTurns() == 0);
    REQUIRE(GetDeckCardCount() == 60);
    REQUIRE(GetDiscardCardCount() == 0);
    REQUIRE(GetPlayerCount() == 1);
    REQUIRE(GetPlayerByIdx(0)->GetName() == "Kept");

    ResetGame();
    REQUIRE(GetNumTurns() == 0);
    REQUIRE(GetDeckCardCount() == 60);
    REQUIRE(GetDiscardCardCount() == 0);
    REQUIRE(GetPlayerCount() == 0);
}

TEST_CASE("RackoDeck prints its full Racko card range", "[RackoDeck][PrintDeck]") {
    RackoDeck deck;
    const std::string printedDeck = deck.PrintDeck();

    REQUIRE(printedDeck.find("---Start Deck---\n") == 0);
    REQUIRE(printedDeck.find("\n--- End Deck ---\n") != std::string::npos);

    const std::size_t cardsStart = printedDeck.find('\n') + 1;
    const std::size_t cardsEnd = printedDeck.find("\n--- End Deck ---");
    std::istringstream cardsStream(printedDeck.substr(cardsStart, cardsEnd - cardsStart));
    std::set<int> cardValues;
    int cardValue = 0;
    while (cardsStream >> cardValue) {
        cardValues.insert(cardValue);
    }

    REQUIRE(cardValues.size() == 60);
    REQUIRE(*cardValues.begin() == 1);
    REQUIRE(*cardValues.rbegin() == 60);
}

TEST_CASE("Player stores named and empty names", "[Player][GetName]") {
    Player namedPlayer("Kathy");
    Player emptyNamePlayer("");

    REQUIRE(namedPlayer.GetName() == "Kathy");
    REQUIRE(emptyNamePlayer.GetName().empty());
}

TEST_CASE_METHOD(TestRacko, "SelectDrawCard accepts a Player pointer",
                 "[Racko][SelectDrawCard]") {
    TestPlayer player("Alex");

    SECTION("Null players and an empty deck return an error") {
        REQUIRE(SelectDrawCard(static_cast<Player*>(nullptr)) == 'e');

        deck = std::make_unique<TestDeck>(std::unordered_set<int>{});
        REQUIRE(SelectDrawCard(&player) == 'e');
    }

    SECTION("A player can choose the deck") {
        std::istringstream input("0\n");
        std::ostringstream output;
        auto* oldInput = std::cin.rdbuf(input.rdbuf());
        auto* oldOutput = std::cout.rdbuf(output.rdbuf());

        const char result = SelectDrawCard(&player);

        std::cin.rdbuf(oldInput);
        std::cout.rdbuf(oldOutput);

        REQUIRE(result == 'r');
        REQUIRE(output.str().find("Choose a card") != std::string::npos);
    }

    SECTION("A player can choose a populated discard pile") {
        Card discardCard(25, Suit::CLUBS);
        discardPile.push(&discardCard);

        std::istringstream input("1\n");
        std::ostringstream output;
        auto* oldInput = std::cin.rdbuf(input.rdbuf());
        auto* oldOutput = std::cout.rdbuf(output.rdbuf());

        const char result = SelectDrawCard(&player);

        std::cin.rdbuf(oldInput);
        std::cout.rdbuf(oldOutput);

        REQUIRE(result == 'i');
        REQUIRE(output.str().find("25") != std::string::npos);
    }

    SECTION("An empty discard pile rejects that choice before accepting the deck") {
        std::istringstream input("1\n0\n");
        std::ostringstream output;
        auto* oldInput = std::cin.rdbuf(input.rdbuf());
        auto* oldOutput = std::cout.rdbuf(output.rdbuf());

        const char result = SelectDrawCard(&player);

        std::cin.rdbuf(oldInput);
        std::cout.rdbuf(oldOutput);

        REQUIRE(result == 'r');
        REQUIRE(output.str().find("no cards in the discard pile") != std::string::npos);
    }
}

TEST_CASE_METHOD(TestRacko, "Racko setup initializes players and difficulty",
                 "[Racko][SetupGame]") {
    std::istringstream input("1\nSolo\n2\n");
    std::ostringstream output;
    auto* oldInput = std::cin.rdbuf(input.rdbuf());
    auto* oldOutput = std::cout.rdbuf(output.rdbuf());

    Racko::SetupGame();

    std::cin.rdbuf(oldInput);
    std::cout.rdbuf(oldOutput);

    REQUIRE(GetNumGames() == 1);
    REQUIRE(GetNumTurns() == 0);
    REQUIRE(GetPlayerCount() == 1);
    REQUIRE(GetPlayerByIdx(0)->GetName() == "Solo");
    REQUIRE(GetPlayerByIdx(0)->GetNumCards() == 10);
    REQUIRE(GetDeckCardCount() == 50);
    REQUIRE(GetDiscardCardCount() == 0);
    REQUIRE(GetScoreGoal() == 150);
}

TEST_CASE_METHOD(TestRacko, "PlayTurnForPlayerByIdx replaces a card and reports rack completion",
                 "[Racko][PlayTurnForPlayerByIdx]") {
    SECTION("A non-winning replacement is discarded and returns zero") {
        auto* player = new TestPlayer("Alpha");
        player->SetCards(std::vector<int>{2, 4, 6, 8, 10, 12, 14, 16, 18, 20});
        AddPlayer(player);
        deck = std::make_unique<Deck>(1, 1);

        std::istringstream input("0\n30\n");
        std::ostringstream output;
        int result = -1;
        {
            ScopedStreamRedirect redirect(input, output);
            result = PlayTurnForPlayerByIdx(0);
        }

        REQUIRE(result == 0);
        REQUIRE(player->GetCardVals() == std::vector<int>{2, 4, 6, 8, 10, 1, 14, 16, 18, 20});
        REQUIRE(GetDiscardCardCount() == 1);
        REQUIRE(GetTopCardFromDiscard()->getValue() == 12);
        ResetPlayers();
    }

    SECTION("A replacement completing an ascending rack returns its rack score") {
        auto* player = new TestPlayer("Alpha");
        player->SetCards(std::vector<int>{2, 3, 4, 5, 6, 7, 8, 9, 10, 11});
        AddPlayer(player);
        deck = std::make_unique<Deck>(1, 1);

        std::istringstream input("0\n5\n");
        std::ostringstream output;
        int result = 0;
        {
            ScopedStreamRedirect redirect(input, output);
            result = PlayTurnForPlayerByIdx(0);
        }

        REQUIRE(result == 75);
        REQUIRE(player->GetCardVals() == std::vector<int>{1, 3, 4, 5, 6, 7, 8, 9, 10, 11});
        REQUIRE(GetDiscardCardCount() == 1);
        REQUIRE(GetTopCardFromDiscard()->getValue() == 2);
        ResetPlayers();
    }
}

TEST_CASE_METHOD(TestRacko, "PlayTurn processes players in order and scores a completed round",
                 "[Racko][PlayTurn]") {
    auto* firstPlayer = new TestPlayer("First");
    firstPlayer->SetCards(std::vector<int>{2, 4, 6, 8, 10, 12, 14, 16, 18, 20});
    auto* winningPlayer = new TestPlayer("Winner");
    winningPlayer->SetCards(std::vector<int>{3, 4, 5, 6, 7, 8, 9, 10, 11, 12});
    auto* unplayedPlayer = new TestPlayer("Unplayed");
    unplayedPlayer->SetCards(std::vector<int>{10, 1, 2, 3, 4, 5, 6, 7, 8, 9});
    AddPlayer(firstPlayer);
    AddPlayer(winningPlayer);
    AddPlayer(unplayedPlayer);
    deck = std::make_unique<Deck>(2, 1);

    std::istringstream input("0\n30\n0\n5\n");
    std::ostringstream output;
    {
        ScopedStreamRedirect redirect(input, output);
        PlayTurn();
    }

    REQUIRE(GetNumTurns() == 1);
    REQUIRE(firstPlayer->GetCardVals() == std::vector<int>{2, 4, 6, 8, 10, 1, 14, 16, 18, 20});
    REQUIRE(winningPlayer->GetCardVals() == std::vector<int>{2, 4, 5, 6, 7, 8, 9, 10, 11, 12});
    REQUIRE(unplayedPlayer->GetCardVals() == std::vector<int>{10, 1, 2, 3, 4, 5, 6, 7, 8, 9});
    REQUIRE(firstPlayer->GetScore() == 25);
    REQUIRE(winningPlayer->GetScore() == 75);
    REQUIRE(unplayedPlayer->GetScore() == 5);
    REQUIRE(GetDiscardCardCount() == 2);
    REQUIRE(GetDeckCardCount() == 0);
    ResetPlayers();
}

TEST_CASE_METHOD(TestRacko, "FinishGame reports rankings and handles play-again input",
                 "[Racko][FinishGame]") {
    SECTION("Zero players can quit without printing nonexistent places") {
        std::istringstream input("N\n");
        std::ostringstream output;
        int result = -1;
        {
            ScopedStreamRedirect redirect(input, output);
            result = FinishGame();
        }

        REQUIRE(result == 0);
        REQUIRE(output.str().find("First Place: \n") == std::string::npos);
        REQUIRE(output.str().find("Second Place:") == std::string::npos);
        REQUIRE(output.str().find("Third Place: ") == std::string::npos);
    }

    SECTION("One player can choose to play again after an invalid response") {
        auto* solo = new TestPlayer("Solo");
        solo->AddScore(20);
        AddPlayer(solo);

        std::istringstream input("x\nY\n");
        std::ostringstream output;
        int result = 0;
        {
            ScopedStreamRedirect redirect(input, output);
            result = FinishGame();
        }

        REQUIRE(result == 1);
        REQUIRE(output.str().find("First Place: Solo\n") != std::string::npos);
        REQUIRE(output.str().find("Second Place:") == std::string::npos);
        ResetPlayers();
    }

    SECTION("Two players are reported in score order") {
        auto* lower = new TestPlayer("Lower");
        lower->AddScore(40);
        auto* higher = new TestPlayer("Higher");
        higher->AddScore(80);
        AddPlayer(lower);
        AddPlayer(higher);

        std::istringstream input("n\n");
        std::ostringstream output;
        int result = -1;
        {
            ScopedStreamRedirect redirect(input, output);
            result = FinishGame();
        }

        REQUIRE(result == 0);
        REQUIRE(output.str().find("First Place: Higher\n") != std::string::npos);
        REQUIRE(output.str().find("Second Place: Lower\n") != std::string::npos);
        ResetPlayers();
    }

    SECTION("Three players are reported in score order") {
        auto* first = new TestPlayer("First");
        first->AddScore(90);
        auto* second = new TestPlayer("Second");
        second->AddScore(60);
        auto* third = new TestPlayer("Third");
        third->AddScore(30);
        AddPlayer(first);
        AddPlayer(second);
        AddPlayer(third);

        std::istringstream input("n\n");
        std::ostringstream output;
        int result = -1;
        {
            ScopedStreamRedirect redirect(input, output);
            result = FinishGame();
        }

        REQUIRE(result == 0);
        CHECK(output.str().find("First Place: First\n") != std::string::npos);
        CHECK(output.str().find("Second Place: Second\n") != std::string::npos);
        CHECK(output.str().find("Third Place: Third\n") != std::string::npos);
        ResetPlayers();
    }

    SECTION("More than three players include the last-place player") {
        auto* first = new TestPlayer("First");
        first->AddScore(90);
        auto* second = new TestPlayer("Second");
        second->AddScore(70);
        auto* third = new TestPlayer("Third");
        third->AddScore(50);
        auto* last = new TestPlayer("Last");
        last->AddScore(10);
        AddPlayer(first);
        AddPlayer(second);
        AddPlayer(third);
        AddPlayer(last);

        std::istringstream input("n\n");
        std::ostringstream output;
        int result = -1;
        {
            ScopedStreamRedirect redirect(input, output);
            result = FinishGame();
        }

        REQUIRE(result == 0);
        CHECK(output.str().find("First Place: First\n") != std::string::npos);
        CHECK(output.str().find("Second Place: Second\n") != std::string::npos);
        CHECK(output.str().find("Third Place: Third\n") != std::string::npos);
        CHECK(output.str().find("Last Place: Last\n") != std::string::npos);
        ResetPlayers();
    }
}

TEST_CASE("Deck card factory creates distinct cards with the requested values",
          "[Deck][MakeNewCard]") {
    InspectableDeck deck(0, 0);

    Card* ace = deck.MakeNewCard(1, Suit::SPADES);
    Card* joker = deck.MakeNewCard(0, Suit::HEARTS);

    REQUIRE(ace != nullptr);
    REQUIRE(ace->getValue() == 1);
    REQUIRE(ace->getSuit() == Suit::SPADES);
    REQUIRE(joker != nullptr);
    REQUIRE(joker->getValue() == 0);
    REQUIRE(joker->getSuit() == Suit::HEARTS);
    REQUIRE(ace != joker);

    delete ace;
    delete joker;
}

TEST_CASE("RackoDeck card factory returns RackoCards at both value boundaries",
          "[RackoDeck][MakeNewCard]") {
    InspectableRackoDeck deck;

    Card* lowest = deck.MakeNewCard(1, Suit::SPADES);
    Card* highest = deck.MakeNewCard(60, Suit::CLUBS);

    REQUIRE(lowest != nullptr);
    REQUIRE(dynamic_cast<RackoCard*>(lowest) != nullptr);
    REQUIRE(lowest->getValue() == 1);
    REQUIRE(lowest->getSuit() == static_cast<Suit>(0));
    REQUIRE(highest != nullptr);
    REQUIRE(dynamic_cast<RackoCard*>(highest) != nullptr);
    REQUIRE(highest->getValue() == 60);
    REQUIRE(highest->getSuit() == static_cast<Suit>(0));
    REQUIRE(lowest != highest);

    delete lowest;
    delete highest;
}

TEST_CASE("TestPlayer provides its documented fixture cards",
          "[TestPlayer][GetTestCardVals]") {
    const std::vector<int> fixtureValues = TestPlayer::GetTestCardVals();
    const std::set<int> fixtureValueSet(fixtureValues.begin(), fixtureValues.end());
    const std::set<int> expectedValues{
        1, 2, 3, 4, 5, 6, 10, 18, 20, 25, 30, 35, 36, 38, 40
    };

    REQUIRE(fixtureValues.size() == 15);
    REQUIRE(fixtureValueSet == expectedValues);
}

TEST_CASE("TestDeck contains the selected Racko fixture cards",
          "[TestDeck][TestDeck]") {
    const std::unordered_set<int> selectedValues{3, 12, 58};
    TestDeck deck(selectedValues);
    const std::vector<Card*> cards = deck.GetCards();

    REQUIRE(cards.size() == selectedValues.size());
    std::set<int> deckValues;
    for (const Card* card : cards) {
        REQUIRE(dynamic_cast<const RackoCard*>(card) != nullptr);
        deckValues.insert(card->getValue());
    }
    REQUIRE(deckValues == std::set<int>{3, 12, 58});
    CHECK(deck.GetNumCards() == selectedValues.size());
}

TEST_CASE("TestDeck contains the selected and ordered Racko fixture cards",
          "[TestDeck][TestDeck]") {
    const std::vector<int> selectedValues{3, 58, 12};
    TestDeck deck(selectedValues);
    const std::vector<Card*> cards = deck.GetCards();    

    REQUIRE(cards.size() == selectedValues.size());
    std::vector<int> deckValuesVector;
    for (const Card* card : cards) {
        REQUIRE(dynamic_cast<const RackoCard*>(card) != nullptr);
        deckValuesVector.push_back(card->getValue());
    }
    REQUIRE(deckValuesVector == std::vector<int>{3, 58, 12});
    CHECK(deck.GetNumCards() == selectedValues.size());
}

TEST_CASE_METHOD(TestRacko, "PlacementForLarger maintains highest and lowest placements",
                 "[Racko][PlacementForLarger]") {
    Player first("First");
    Player second("Second");
    Player third("Third");
    first.AddScore(100);
    second.AddScore(60);
    third.AddScore(20);

    Player* placed = nullptr;
    REQUIRE(PlacementForLarger(placed, &first, false) == nullptr);
    REQUIRE(placed == &first);

    Player* unplaced = PlacementForLarger(placed, &second, false);
    REQUIRE(placed == &first);
    REQUIRE(unplaced == &second);

    unplaced = PlacementForLarger(placed, &third, false);
    REQUIRE(placed == &first);
    REQUIRE(unplaced == &third);

    Player* candidate = nullptr;
    REQUIRE(PlacementForLarger(candidate, &first, true) == nullptr);
    REQUIRE(candidate == &first);

    unplaced = PlacementForLarger(candidate, &third, true);
    REQUIRE(candidate == &third);
    REQUIRE(unplaced == &first);

    unplaced = PlacementForLarger(candidate, &second, true);
    REQUIRE(candidate == &third);
    REQUIRE(unplaced == &second);

    Player higher("Higher");
    higher.AddScore(120);
    unplaced = PlacementForLarger(placed, &higher, false);
    REQUIRE(placed == &higher);
    REQUIRE(unplaced == &first);
}

TEST_CASE("RackoPlayer orders players by score and then name",
          "[RackoPlayer][operator<]") {
    RackoPlayer lowerScore("Zulu");
    RackoPlayer higherScore("Alpha");
    lowerScore.AddScore(10);
    higherScore.AddScore(20);

    CHECK(lowerScore < higherScore);
    CHECK_FALSE(higherScore < lowerScore);

    RackoPlayer earlierName("Alex");
    RackoPlayer laterName("Blair");
    earlierName.AddScore(20);
    laterName.AddScore(20);

    CHECK(earlierName < laterName);
    CHECK_FALSE(laterName < earlierName);
    CHECK_FALSE(earlierName < earlierName);
}

TEST_CASE_METHOD(TestRacko, "Racko player-name lookup handles duplicate and unknown names",
                 "[Racko][GetPlayerIdxByName]") {
    AddPlayer("Alpha");
    AddPlayer("Bravo");
    AddPlayer("Alpha");
    AddPlayer("");

    REQUIRE(GetPlayerIdxByName("Alpha") == 0);
    REQUIRE(GetPlayerIdxByName("Bravo") == 1);
    REQUIRE(GetPlayerIdxByName("") == 3);
    CHECK(GetPlayerIdxByName("Missing") == -1);
    CHECK(GetPlayerByName("Missing") == nullptr);
}

TEST_CASE_METHOD(TestRacko, "PlayTurnForPlayerByIdx handles invalid players and draw sources",
                 "[Racko][PlayTurnForPlayerByIdx]") {
    SECTION("Null and non-Racko players return their documented errors") {
        AddPlayer(static_cast<Player*>(nullptr));
        REQUIRE(PlayTurnForPlayerByIdx(0) == -1);

        ResetPlayers();
        AddPlayer(new Player("Base"));
        REQUIRE(PlayTurnForPlayerByIdx(0) == -2);
        ResetPlayers();
    }

    SECTION("An empty deck reports a draw-selection error") {
        auto* player = new TestPlayer("Empty");
        player->SetCards(std::vector<int>{1, 3, 5});
        AddPlayer(player);
        deck = std::make_unique<Deck>(0, 0);

        REQUIRE(PlayTurnForPlayerByIdx(0) == -3);
        REQUIRE(player->GetCardVals() == std::vector<int>{1, 3, 5});
        ResetPlayers();
    }

    SECTION("A discard-pile draw replaces the selected rack position") {
        auto* player = new TestPlayer("Discard");
        player->SetCards(std::vector<int>{2, 4, 6, 8, 10, 12, 14, 16, 18, 20});
        AddPlayer(player);
        deck = std::make_unique<Deck>(1, 1);
        AddCardToDiscard(new RackoCard(50));

        std::istringstream input("1\n30\n");
        std::ostringstream output;
        int result = -1;
        {
            ScopedStreamRedirect redirect(input, output);
            result = PlayTurnForPlayerByIdx(0);
        }

        REQUIRE(result == 0);
        REQUIRE(player->GetCardVals() == std::vector<int>{2, 4, 6, 8, 10, 50, 14, 16, 18, 20});
        REQUIRE(GetDiscardCardCount() == 1);
        REQUIRE(GetTopCardFromDiscard()->getValue() == 12);
        ResetPlayers();
    }

    SECTION("An out-of-range player index throws") {
        REQUIRE_THROWS_AS(PlayTurnForPlayerByIdx(0), std::out_of_range);
    }
}

TEST_CASE_METHOD(TestRacko, "PlayTurnForPlayerByName resolves duplicate and empty names",
                 "[Racko][PlayTurnForPlayerByName]") {
    SECTION("A duplicate name resolves to the first matching player") {
        auto* first = new TestPlayer("Twin");
        first->SetCards(std::vector<int>{2, 4, 6, 8, 10, 12, 14, 16, 18, 20});
        auto* second = new TestPlayer("Twin");
        second->SetCards(std::vector<int>{3, 5, 7, 9, 11, 13, 15, 17, 19, 21});
        AddPlayer(first);
        AddPlayer(second);
        deck = std::make_unique<Deck>(1, 1);

        std::istringstream input("0\n30\n");
        std::ostringstream output;
        int result = -1;
        {
            ScopedStreamRedirect redirect(input, output);
            result = PlayTurnForPlayerByName("Twin");
        }

        REQUIRE(result == 0);
        REQUIRE(first->GetCardVals().at(5) == 1);
        REQUIRE(second->GetCardVals() == std::vector<int>{3, 5, 7, 9, 11, 13, 15, 17, 19, 21});
        ResetPlayers();
    }

    SECTION("An empty name resolves to the matching player") {
        auto* player = new TestPlayer("");
        player->SetCards(std::vector<int>{1, 3, 5});
        AddPlayer(player);
        deck = std::make_unique<Deck>(0, 0);

        REQUIRE(PlayTurnForPlayerByName("") == -3);
        ResetPlayers();
    }

    SECTION("An unknown name propagates the invalid-index exception") {
        REQUIRE_THROWS_AS(PlayTurnForPlayerByName("Missing"), std::out_of_range);
    }
}
