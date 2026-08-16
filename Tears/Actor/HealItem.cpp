#include "HealItem.h"
#include <Actor/Player.h>
#include <Engine/Engine.h>
using namespace Craft;
HealItem::HealItem(const Craft::Vector2& position) : Actor("+", position, Color::Green)
{
}

void HealItem::OnCollision(const std::shared_ptr<Craft::Actor>& other)
{
    super::OnCollision(other);

    if (std::shared_ptr<Player> player = Cast<Player>(other))
    {
        Engine::Get().PlayOneShot("powerUp.wav");
        player->Heal(heal);
        Destroy();
    }
}
