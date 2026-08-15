#include "Room3.h"
#include <Level/BossRoom.h>
#include <Engine/Engine.h>
#include <Util/Util.h>

using namespace Craft;

void Room3::SpawnDoor()
{
    Vector2 rightDoorPosition(Engine::Get().GetWidth() - 1, Engine::Get().GetHeight() / 2 - (Door::doorLength / 2));

    //오른쪽 문 진입시 다음 레벨에서는 왼쪽에서 시작
    TrackSpawnedDoor<Door>(rightDoorPosition, []()
        {
            Engine::Get().GetGameInstance<RunState>()->entryDirection = EntryDirection::Left;
            Engine::Get().AddNewLevel<BossRoom>();
        },
        DoorDirection::Vertical
    );
}

void Room3::SpawnEnemies()
{
    int enemyCount = Util::RandomRange(3, 7);

    for (int i = 0; i < enemyCount; i++)
    {
        int x = Util::RandomRange(1, Engine::Get().GetWidth() - 6);
        int y = Util::RandomRange(1, Engine::Get().GetHeight() - 6);
        TrackSpawnedEnemy<Enemy>(Vector2(x, y));
    }
}
