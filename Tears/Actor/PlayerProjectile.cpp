#include "PlayerProjectile.h"

using namespace Craft;
PlayerProjectile::PlayerProjectile(const Craft::Vector2& position) 
    : Actor("@", position, Color::Blue), yPosition(static_cast<float>(position.y))
{
}

void PlayerProjectile::Tick(float deltaTime)
{
    super::Tick(deltaTime);

    //아래에서 위로 이동 처리
    yPosition -= moveSpeed * deltaTime;

    //좌표 검사 (화면 밖으로 벗어났는지 확인)
    if (yPosition <0.0f)
    {
        //삭제 처리
        Destroy();
        return;
    }

    //위치 값 설정 및 갱신
    Vector2 newPosition = GetPosition();
    newPosition.y = static_cast<int>(yPosition);
    SetPosition(newPosition);
    
}
