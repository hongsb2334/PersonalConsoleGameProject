#pragma once
#include <Actor/Actor.h>

class Obstacle : public Craft::Actor
{
    TYPE_DECLARATIONS(Obstacle, Actor)

public:
    Obstacle(const Craft::Vector2& position, int tileW, int tileH);


    virtual void Draw() override;

private:

};

