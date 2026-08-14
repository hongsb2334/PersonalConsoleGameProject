#include "StartRoom.h"
#include "Actor/Door.h"
#include <Engine/Engine.h>
#include <Level/Room2.h>
#include <Core/RunState.h>
using namespace Craft;

void StartRoom::SpawnDoor()
{
    //문을 화면 중앙 상단에 배치할 것임
    Vector2 doorPosition(Engine::Get().GetWidth() / 2, 0);

    TrackSpawnedDoor<Door>(doorPosition, []()
        {
            Engine::Get().GetGameInstance<RunState>()->entryDirection = EntryDirection::Bottom;
            Engine::Get().AddNewLevel<Room2>();
        }
    );
}
