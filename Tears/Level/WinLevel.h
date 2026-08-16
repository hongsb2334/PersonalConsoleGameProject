#pragma once
#include <Level/ResultLevel.h>

class WinLevel : public ResultLevel
{
    TYPE_DECLARATIONS(WinLevel, ResultLevel)

public:
    WinLevel() : ResultLevel("You Win!", Craft::Color::Yellow, "Wingame.wav") {}

};