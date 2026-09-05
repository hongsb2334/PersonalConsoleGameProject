#include "BossRoom.h"
#include <Engine/Engine.h>
#include <Util/Util.h>
#include <Level/WinLevel.h>
#include <Actor/Boss.h>
#include <Actor/WandererEnemy.h>
#include <Actor/OrbiterEnemy.h>
#include <Actor/ChargerEnemy.h>
#include <Render/Renderer.h>
using namespace Craft;

void BossRoom::SpawnDoor()
{
}

void BossRoom::SpawnEnemies()
{
    //Todo: ChargerEnemy가 보스전에서 출현하도록 구성, 후에 수정
    int x = Util::RandomRange(1, Engine::Get().GetWidth() - 6);
    int y = Util::RandomRange(1, Engine::Get().GetHeight() - 6);
    //TrackSpawnedEnemy<Boss>(Vector2(x, y));
    TrackSpawnedEnemy<ChargerEnemy>(Vector2(x, y));
    
}

void BossRoom::OnRoomCleared()
{
    super::OnRoomCleared();
    Engine::Get().AddNewLevel<WinLevel>();
}

void BossRoom::Draw()
{
    super::Draw();

    //std::shared_ptr<Boss> boss = FindActor<Boss>();
    std::shared_ptr<ChargerEnemy> boss = FindActor<ChargerEnemy>();
    if (boss)
    {
        std::string bosshpText = "BOSS HP : " + std::to_string(boss->GetHp()) + " / " + std::to_string(boss->GetMaxHp());
        int textX = Engine::Get().GetWidth() - static_cast<int>(bosshpText.length()) - 1;
        Renderer::Get().Submit(bosshpText, Vector2(textX, 0), Color::Red);
    }
}
