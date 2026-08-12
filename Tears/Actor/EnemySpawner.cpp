#include "EnemySpawner.h"
#include <Util/Util.h>
#include <Engine/Engine.h>
#include <Actor/Enemy.h>
#include <Level/Level.h>
using namespace Craft;

EnemySpawner::EnemySpawner()
{
    timer.SetTargetTime(Util::RandomRange(1.0f, 3.0f));
}

void EnemySpawner::Tick(float deltaTime)
{
    super::Tick(deltaTime);

    //타이머 업데이트
    timer.Tick(deltaTime);

    //경과 시간 확인
    if (!timer.IsTimeOut())
    {
        return;
    }
    
    //타이머 초기화
    timer.Reset();
    
    //적 생성
    SpawnEnemy();
}

void EnemySpawner::SpawnEnemy()
{
    std::shared_ptr<Level> owner = GetOwner();
    if (!owner)
    {
        return;
    }

    //화면 전체 범위에서 X, Y 모두 랜덤 위치 선정
    //RandomRange는 max까지 포함이라 -1 해주기
    int xPosition = Util::RandomRange(0, Engine::Get().GetWidth() - 1);
    int yPosition = Util::RandomRange(0, Engine::Get().GetHeight() - 1);


    owner->SpawnActor<Enemy>(Vector2(xPosition, yPosition));
}
