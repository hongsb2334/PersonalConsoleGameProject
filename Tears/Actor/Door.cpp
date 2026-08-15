#include "Door.h"
#include <Actor/Player.h>
#include <Engine/Engine.h>
#include <Core/RunState.h>
#include <Render/Renderer.h>

using namespace Craft;

Door::Door(const Craft::Vector2& position, std::function<void()> onEnter, DoorDirection doorDirection)
    : Actor("#######", position, Craft::Color::Green), onEnter(onEnter), doorDirection(doorDirection)
{
    if (doorDirection == DoorDirection::Vertical)
    {
        width = 1;
        height = static_cast<int>(image.length());
    }
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

    if (doorDirection == DoorDirection::Vertical)
    {
        //한 줄로 되어있는 문을 한글자씩 그림
        for (int i = 0; i < static_cast<int>(image.length()); ++i)
        {
            Renderer::Get().Submit(std::string(1, image[i]), Vector2(GetPosition().x, GetPosition().y + i), color, sortingOrder);
        }
    }
    else
    {
        super::Draw();
    }
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
