#pragma once
#include <Actor/Actor.h>

class HealItem : public Craft::Actor
{
    TYPE_DECLARATIONS(HealItem, Actor)

public:
    HealItem(const Craft::Vector2& position);
    
protected:
    virtual void OnCollision(const std::shared_ptr<Craft::Actor>& other) override;

private:
    int heal = 2;

};

