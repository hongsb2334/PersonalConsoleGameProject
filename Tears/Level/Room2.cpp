#include "Room2.h"
#include <Level/Room3.h>
#include <Level/Room4.h>
#include <Engine/Engine.h>
#include <Util/Util.h>

using namespace Craft;

void Room2::SpawnDoor()
{
    Vector2 topDoorPosition(Engine::Get().GetWidth() / 2, 0);
    Vector2 rightDoorPosition(Engine::Get().GetWidth() - 1, Engine::Get().GetHeight() / 2);

    //위쪽 문으로 나갈 시 다음 레벨의 아래쪽부터 시작
    TrackSpawnedDoor<Door>(topDoorPosition, []()
        {
            Engine::Get().GetGameInstance<RunState>()->entryDirection = EntryDirection::Bottom;
            Engine::Get().AddNewLevel<Room3>();
        }
    );
    
    //오른쪽 문으로 나갈 시 다음 레벨의 왼쪽부터 시작
    TrackSpawnedDoor<Door>(rightDoorPosition, []()
        {
            Engine::Get().GetGameInstance<RunState>()->entryDirection = EntryDirection::Left;
            Engine::Get().AddNewLevel<Room4>();
        },
        DoorDirection::Vertical
    );
}

void Room2::SpawnEnemies()
{
    int enemyCount = Util::RandomRange(3, 7);

    for (int i = 0; i < enemyCount; i++)
    {
        //Todo: 화면 가장자리 짤리는 현상 일어날 수 있어 확인 필요
        int x = Util::RandomRange(1, Engine::Get().GetWidth() - 6);
        int y = Util::RandomRange(1, Engine::Get().GetHeight() - 6);

        TrackSpawnedEnemy<Enemy>(Vector2(x, y));
    }
}
