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

    //x값이 0보다 작아지면 0으로 클램프 후 부호 바꿔서 반대로 진행하게 함
    if (xPosition < 0)
    {
        xPosition = 0.0f;
    }
    //x값과 가로 길이 더한게 창 넘어가면 창에서 글자 길이 빼서 클램프 해주고 방향 반대 설정
    if (xPosition + width >= Engine::Get().GetWidth())
    {
        xPosition = Engine::Get().GetWidth() - width;
    }
    //x값과 마찬가지
    if (yPosition < 0)
    {
        yPosition = 0;
    }
    //Todo: x값이랑 똑같은데 현재 height는 1이다, 나중에 2차원 액터 사용시 수정 필요
    if (yPosition + height >= Engine::Get().GetHeight())
    {
        yPosition = Engine::Get().GetHeight() - height;
    }
    
    //Todo: 클램프 직후 xPosition과 액터의 실제 위치가 한프레임 어긋나 있음(실제로 눈에 안보임). 나중에 문제 생기면 이 코드 활성화  
    //Vector2 clamp = GetPosition();
    //clamp.x = static_cast<int>(xPosition);
    //clamp.y = static_cast<int>(yPosition);
    //SetPosition(clamp);
    
}
