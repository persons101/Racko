#include "../../src/racko.h"

class TestRacko : public Racko {
public:
    using Difficulty = Racko::Difficulty;
    using Racko::ChooseDifficulty;
    using Racko::DrawCardForPlayer;
    using Racko::GetPlayerIdxByName;
    using Racko::PlayTurnForPlayerByName;
    using Racko::ResetPlayers;
    using Racko::SetCustomGoal;
    using Racko::SetDifficulty;
    using Racko::SetScoreGoal;

    void SetupGame() override;
};