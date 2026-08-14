#include "BossRoom.h"
#include <Engine/Engine.h>
#include <Util/Util.h>
using namespace Craft;

void BossRoom::SpawnDoor()
{
}

void BossRoom::SpawnEnemies()
{
    int x = Util::RandomRange(1, Engine::Get().GetWidth() - 6);
    int y = Util::RandomRange(1, Engine::Get().GetHeight() - 6);
    TrackSpawnedEnemy<Enemy>(Vector2(x, y));
}
