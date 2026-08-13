#include "StartRoom.h"
#include "Actor/Player.h"
#include <Engine/Engine.h>
#include <Level/Room2.h>
using namespace Craft;
void StartRoom::OnInitialized()
{
    super::OnInitialized();

    SpawnActor<Player>();

    SpawnDoor();
}

void StartRoom::SpawnDoor()
{
    //문을 화면 중앙 상단에 배치할 것임
    Vector2 doorPosition(Engine::Get().GetWidth() / 2, 0);

    TrackSpawnedDoor<Door>(doorPosition, []()
        {
            Engine::Get().AddNewLevel<Room2>();
        }
    );
}
