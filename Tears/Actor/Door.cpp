#include "Door.h"
#include <Actor/Player.h>
#include <Engine/Engine.h>
#include <Core/RunState.h>

using namespace Craft;

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

void Door::OnCollision(const std::shared_ptr<Actor>& other)
{
    super::OnCollision(other);
    if (!isOpen)
    {
        return;
    }

    if (auto player = Cast<Player>(other))
    {
        Engine::Get().GetGameInstance<RunState>()->playerHp = player->GetHp();
    }

    if (other->IsTypeOf<Player>() && onEnter)
    {
        onEnter();
    }
}
