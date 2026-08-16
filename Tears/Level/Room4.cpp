#include "Room4.h"
#include "BossRoom.h"
#include <Engine/Engine.h>
#include <Util/Util.h>
using namespace Craft;

void Room4::SpawnDoor()
{
    Vector2 topDoorPosition(Engine::Get().GetWidth() / 2 - (Door::doorLength / 2), 0);

    //오른쪽 문 진입시 다음 레벨에서는 왼쪽에서 시작
    TrackSpawnedDoor<Door>(topDoorPosition, []()
        {
            Engine::Get().GetGameInstance<RunState>()->entryDirection = EntryDirection::Bottom;
            Engine::Get().AddNewLevel<BossRoom>();
        }
    );
}

void Room4::SpawnEnemies()
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
