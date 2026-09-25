#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"

#include <iostream>
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

class DifficultyTestRacko : public Racko {
public:
    using Difficulty = Racko::Difficulty;
    using Racko::ChooseDifficulty;
    using Racko::SetCustomGoal;
    using Racko::SetDifficulty;
    using Racko::SetScoreGoal;
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
        cards.push_back(new Card(i, Suit(i % 4)));
    }
    Card* eightOfClubs = cards.at(8);
    REQUIRE(eightOfClubs->getSuit() == Suit::CLUBS);
    REQUIRE(eightOfClubs->getValue() == 8);

    std::tuple<int, Suit> cardTuple = std::tuple(11, Suit::DIAMONDS);
    Card* jackOfDiamonds = new Card(cardTuple);
    REQUIRE(jackOfDiamonds);
    for (int i = 12; i <= 14; i++) {
        cards.push_back(new Card(i, Suit::DIAMONDS));
    }

    for (std::size_t i = 0; cards.size(); i++) {
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
        REQUIRE(eightOfClubs->getSuit() == Suit::CLUBS);
        REQUIRE(eightOfClubs->getSuitName() == "Clubs");
        REQUIRE(aceOfSpades->getSuitSymbol() == "\xe2\x99\xA3");
        REQUIRE(jackOfDiamonds->getSuit() == Suit::DIAMONDS);
        REQUIRE(jackOfDiamonds->getSuitName() == "Diamonds"); 
        REQUIRE(aceOfSpades->getSuitSymbol() == "\xe2\x99\xA6");
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
    REQUIRE( ( (*topCard) == std::make_tuple(1, (Suit)0)) );
}

TEST_CASE("RackoCard vs Card", "[RackoCard][cout]")
{
    /// GOAL: Create RackoCards with values 1, 11, 12, and 13, (usually A, J, Q, K), and confirm they print numbers and not letters. This ensures the inheritance is working

    // Save original buffer
    auto old_buf = std::cout.rdbuf();

    // Redirect cout to a stringstream
    std::stringstream oss;
    std::cout.rdbuf(oss.rdbuf());

    // Execute code that writes to cout
    auto cardValue = GENERATE(1, 11, 12, 13);

    RackoCard rackoCard(cardValue);
    rackoCard.PrintCardShort();


    // Restore original buffer
    std::cout.rdbuf(old_buf);

    // Check the captured output
    REQUIRE(oss.str() == (std::to_string(rackoCard.getValue()) ) );
}

TEST_CASE_METHOD(Racko, "TestPlayer functions", "[TestPlayer][Racko]") {
    TestPlayer p1;
    std::vector<Card*> cards = p1.GetCards(); // 1,5,10,15,20,25,30,35,36,38,40,2,3,4,6 
    REQUIRE(CalculateRackScore(cards) == 11 * 5);
    REQUIRE(p1.AddScore(CalculateRackScore(cards)) == 55);
    REQUIRE(p1.GetScore() == 55);

}

TEST_CASE_METHOD(Racko, "Racko game initialization", "[Racko]") {
    REQUIRE(GetNumTurns() == 0);
    REQUIRE(GetDeckCardCount() == 60);
    REQUIRE(GetDiscardCardCount() == 0);
    REQUIRE(GetPlayerCount() == 0);
}

TEST_CASE_METHOD(Racko, "Racko game score calculation", "[Racko][RackoCard][Score][Player]") {
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

        TestDeck* testDeck = new TestDeck(); // top on left: 11,12,13,14,15
        deck = testDeck;
        PlayTurnForPlayerByIdx(0);

        REQUIRE(GetPlayerByIdx(0)->GetCardVals() == std::vector({1,5,10,11,20,25,30,35,36,38,40,2,3,4,6 }));
    }

}


//  Racko::SelectDrawCard() -> 
//  - test when discardPile is empty
//  - check newest card drawn is correct card
//  - test garbage characters
//  - test long strings
//  = test expected behaviors

TEST_CASE_METHOD(Racko, "SelectDrawCard returns error for invalid player",
                 "[Racko][SelectDrawCard]") {
    REQUIRE(SelectDrawCard(-1) == 'e');
}

TEST_CASE_METHOD(Racko, "SelectDrawCard chooses the deck",
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

TEST_CASE_METHOD(Racko, "SelectDrawCard chooses the discard pile",
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

TEST_CASE_METHOD(Racko, "SelectDrawCard rejects invalid input before deck choice",
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

TEST_CASE_METHOD(Racko, "SelectDrawCard rejects discard choice when pile is empty",
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

TEST_CASE_METHOD(Racko, "SelectDrawCard accepts discard choice when pile is not empty",
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

TEST_CASE_METHOD(Racko, "SelectDrawCard rejects multi-character choices",
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
    auto* oldOutput = std::cout.rdbuf(output.rdbuf());
    deck.PrintDeck();
    std::cout.rdbuf(oldOutput);
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

TEST_CASE_METHOD(DifficultyTestRacko, "Racko difficulty and score goal settings",
                 "[Racko][Difficulty][ScoreGoal]") {
    SECTION("Direct score goals include boundaries and negative values") {
        SetScoreGoal(0);
        REQUIRE(GetTestScoreGoal() == 0);

        SetScoreGoal(150);
        REQUIRE(GetTestScoreGoal() == 150);

        SetScoreGoal(-1);
        REQUIRE(GetTestScoreGoal() == -1);
    }

    SECTION("Difficulty state and custom goal are stored") {
        SetCustomGoal(1);
        REQUIRE(GetTestCustomGoal() == 1);
        SetDifficulty(Difficulty::Easy);
        REQUIRE(GetTestDifficulty() == 0);
        SetDifficulty(Difficulty::Custom);
        REQUIRE(GetTestDifficulty() == 3);
    }

    SECTION("Preset difficulties select their documented score goals") {
        SetScoreGoal(Difficulty::Easy);
        REQUIRE(GetTestScoreGoal() == 100);

        SetScoreGoal(Difficulty::Medium);
        REQUIRE(GetTestScoreGoal() == 150);

        SetScoreGoal(Difficulty::Hard);
        REQUIRE(GetTestScoreGoal() == 200);

        SetCustomGoal(75);
        SetScoreGoal(Difficulty::Custom);
        REQUIRE(GetTestScoreGoal() == 75);
    }
}

TEST_CASE_METHOD(DifficultyTestRacko, "Racko difficulty selection handles invalid input",
                 "[Racko][Difficulty][ScoreGoal]") {
    std::istringstream input("x\n0\n2\n");
    auto* oldInput = std::cin.rdbuf(input.rdbuf());

    ChooseDifficulty();

    std::cin.rdbuf(oldInput);
    REQUIRE(GetTestDifficulty() == 1);
    REQUIRE(GetTestScoreGoal() == 150);
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
    REQUIRE(player.center("x", 4, '.') == "...x");
    REQUIRE(player.center("text", 4) == "text");
    REQUIRE(player.center("long text", 4) == "long text");
}

TEST_CASE("RackoPlayer displays rack values and positions", "[RackoPlayer]") {
    TestPlayer player("Alex");
    player.ResetPlayer();

    REQUIRE(player.DisplayCardsInRack() == "\n");

    player.DrawCard(new Card(1, Suit::SPADES));
    player.DrawCard(new Card(10, Suit::HEARTS));
    player.DrawCard(new Card(13, Suit::CLUBS));

    const std::string display = player.DisplayCardsInRack();
    REQUIRE(display.find(" 1  ") != std::string::npos);
    REQUIRE(display.find(" 10 ") != std::string::npos);
    REQUIRE(display.find(" 13 ") != std::string::npos);
    REQUIRE(display.find(" 0  ") != std::string::npos);
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
    player.ResetPlayer();
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

    const int selected = player.SelectCardToReplace(nullptr);

    std::cin.rdbuf(oldInput);
    std::cout.rdbuf(oldOutput);

    REQUIRE(selected == 2);
    REQUIRE(output.str().find("Choose a card to remove") != std::string::npos);
}

TEST_CASE_METHOD(TestPlayer, "RackoPlayer centers text numbers", "[RackoPlayer][center]") {
    std::string twoChar = "ab";
    std::string threeChar = "abc";
    std::string fourChar = "abcd";
    REQUIRE(center(twoChar, 4) == " ab ");
    REQUIRE(center(threeChar, 4) == " abc");
    REQUIRE(center(fourChar, 4) == "abcd");
}

