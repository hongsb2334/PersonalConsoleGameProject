#pragma once
#include <Level/Level.h>

class TitleLevel : public Craft::Level
{
    TYPE_DECLARATIONS(TitleLevel, Level)

protected:
    virtual void OnInitialized() override;
    virtual void Tick(float deltaTime) override;

};

