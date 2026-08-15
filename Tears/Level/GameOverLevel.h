#pragma once
#include <Level/ResultLevel.h>

class GameOverLevel : public ResultLevel
{
    TYPE_DECLARATIONS(GameOverLevel, ResultLevel)

public:
    GameOverLevel() : ResultLevel("Game Over", Craft::Color::Red) {}

};

