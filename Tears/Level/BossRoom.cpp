#include "BossRoom.h"
#include <Engine/Engine.h>
#include <Util/Util.h>
#include <Level/WinLevel.h>
#include <Actor/Boss.h>
#include <Actor/WandererEnemy.h>
#include <Actor/OrbiterEnemy.h>
#include <Actor/ChargerEnemy.h>
#include <Actor/AStarEnemy.h>
#include <Render/Renderer.h>
using namespace Craft;

void BossRoom::SpawnDoor()
{
}

void BossRoom::SpawnEnemies()
{
    int orbiterEnemyCount = Util::RandomRange(1, 3);
    int wandererEnemyCount = Util::RandomRange(1, 3);
    int AStarEnemyCount = Util::RandomRange(1, 3);
    int BossCount = 1;

    SpawnEnemyRandomly<OrbiterEnemy>(orbiterEnemyCount);
    SpawnEnemyRandomly<WandererEnemy>(wandererEnemyCount);
    SpawnEnemyRandomly<AStarEnemy>(AStarEnemyCount);
    SpawnEnemyRandomly<Boss>(BossCount);   
}

void BossRoom::OnRoomCleared()
{
    super::OnRoomCleared();
    Engine::Get().AddNewLevel<WinLevel>();
}

void BossRoom::Draw()
{
    super::Draw();

    //보스 찾아서 보스 체력 표시
    std::shared_ptr<Boss> boss = FindActor<Boss>();
    if (boss)
    {
        std::string bosshpText = "BOSS HP : " + std::to_string(boss->GetHp()) + " / " + std::to_string(boss->GetMaxHp());
        int textX = Engine::Get().GetWidth() - static_cast<int>(bosshpText.length()) - 1;
        Renderer::Get().Submit(bosshpText, Vector2(textX, 0), Color::Red);
    }
}
