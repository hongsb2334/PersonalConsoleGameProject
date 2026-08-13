#include "Door.h"
#include <Actor/Player.h>
Door::Door(const Craft::Vector2& position, std::function<void()> onEnter)
    : Actor("+++++", position, Craft::Color::Green), onEnter(onEnter)
{
}

void Door::Open()
{
    isOpen = true;
}

void Door::Draw()
{
    if (!isOpen)
    {
        return;
    }

    super::Draw();
}

void Door::OnCollision(const std::shared_ptr<Craft::Actor>& other)
{
    super::OnCollision(other);

    if (other->IsTypeOf<Player>() && onEnter)
    {
        onEnter();
    }
}
