#include "OrbiterEnemy.h"
#include <Engine/Engine.h>
#include <Level/Level.h>
#include <Util/Util.h>
#include "Player.h"

using namespace Craft;
OrbiterEnemy::OrbiterEnemy(const Craft::Vector2& position, int maxHp, int damage) : Enemy(position, maxHp, damage)
{
    ChangeImage("Orbit");
    //Todo: 색깔도 바꾸는 로직 추가
    color = Color::Blue;
   
}

void OrbiterEnemy::MovingPattern(float& outDx, float& outDy) const
{
    outDx = 0.0f;
    outDy = 0.0f;

    std::shared_ptr<Level> owner = GetOwner();
    if (!owner)
    {
        return;
    }

    std::shared_ptr<Player> player = owner->FindActor<Player>();
    if (!player)
    {
        return;
    }

    //OrbiterEnemy에서 Player의 방향벡터
    outDx = static_cast<float>(player->GetPosition().x - GetPosition().x);
    outDy = static_cast<float>(player->GetPosition().y - GetPosition().y);

    float length = std::sqrt(outDx * outDx + outDy * outDy);
    if (length <= 0)
    {
        //플레이어와의 거리가 0이면 이동 안함
        outDx = 0.0f;
        outDy = 0.0f;
        return;
    }
    

    //플레이어 방향
    float playerDiretionX = outDx / length;
    float playerDiretionY = outDy / length;

    //90도 회전하는 효과
    float rotateX = -playerDiretionY;
    float rotateY = playerDiretionX;

    //일정 거리 유지
    float maintainDistance = (length - radius) * orbitStat;
    outDx = playerDiretionX * maintainDistance + rotateX;
    outDy = playerDiretionY * maintainDistance + rotateY;
    
    length = std::sqrt(outDx * outDx + outDy * outDy);
    if (length > 0.0f)
    {
        outDx /= length;
        outDy /= length;
    }
    

}

void OrbiterEnemy::Tick(float deltaTime)
{
    super::Tick(deltaTime);

    Clamp();
    
    //Todo: 클램프 직후 xPosition과 액터의 실제 위치가 한프레임 어긋나 있음(실제로 눈에 안보임). 나중에 문제 생기면 이 코드 활성화  
    //Vector2 clamp = GetPosition();
    //clamp.x = static_cast<int>(xPosition);
    //clamp.y = static_cast<int>(yPosition);
    //SetPosition(clamp);
    
}
