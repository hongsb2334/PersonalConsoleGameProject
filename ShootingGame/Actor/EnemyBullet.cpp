#include "EnemyBullet.h"
#include <Engine/Engine.h>
using namespace Craft;
EnemyBullet::EnemyBullet(const Craft::Vector2& position, float moveSpeed) : Actor("#", position, Color::Red), 
        moveSpeed(moveSpeed), yPosition(static_cast<float>(position.y))
{
}

void EnemyBullet::Tick(float deltaTime)
{
    super::Tick(deltaTime);

    //y위치 업데이트(아래로 이동 처리)
    yPosition += moveSpeed * deltaTime;     //1 곱해줘야 하는데 생략했음

    //좌표 검사
    if (yPosition >= Engine::Get().GetHeight() - 1) //y 포지션 - 1 까지 해야 
    {
        Destroy();
        return;
    }

    //위치 설정
    SetPosition(Vector2(GetPosition().x, static_cast<int>(yPosition)));
}
