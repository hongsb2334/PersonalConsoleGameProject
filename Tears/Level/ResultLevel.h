#pragma once
#include <Level/Level.h>
#include <UI/SelectMenu.h>
#include <memory>

class ResultLevel : public Craft::Level
{
    TYPE_DECLARATIONS(ResultLevel, Level)

public:
    ResultLevel(const std::string& title, Craft::Color titleColor, const std::string& filename);

    virtual void Tick(float deltaTime) override;
    virtual void Draw() override;

private:
    std::string title;
    Craft::Color titleColor;
    std::shared_ptr<SelectMenu> menu;
};
