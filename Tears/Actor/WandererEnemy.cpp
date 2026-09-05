#include "WandererEnemy.h"
#include <Engine/Engine.h>
#include <Util/Util.h>
using namespace Craft;
//Todo: 여기서 currentDx를 0으로 초기화하면 게임이 시작되고 처음 4초는 가만히 있다가 움직임, 체크 후 수정
WandererEnemy::WandererEnemy(const Craft::Vector2& position, int maxHp, int damage) : Enemy(position, maxHp, damage), currentDx(0), currentDy(0)
{
    ChangeImage("Wanderer");
    //Todo: 색깔도 바꾸는 로직 추가
    
    //4초가 지나면 방향 무작위로 뽑아서 이동하게 구현
    timer.SetTargetTime(2.0f);
}

//MovingPattern은 const라 멤버변수 currentDx, currentDy 못바꾼다. Tick에서 랜덤 방향 계산해서 여기에 넘겨줌
void WandererEnemy::MovingPattern(float& outDx, float& outDy) const
{   
    outDx = currentDx;
    outDy = currentDy;

    float length = std::sqrt(outDx * outDx + outDy * outDy);
    if (length > 0.0f)
    {
        outDx /= length;
        outDy /= length;
    }
}

void WandererEnemy::Tick(float deltaTime)
{
    super::Tick(deltaTime);
    timer.Tick(deltaTime);
    
    if (timer.IsTimeOut())
    {
        currentDx = Util::RandomRange(-1.0f, 1.0f);
        currentDy = Util::RandomRange(-1.0f, 1.0f);
        timer.Reset();
    }

    //x값이 0보다 작아지면 0으로 클램프 후 부호 바꿔서 반대로 진행하게 함
    if (xPosition < 0)
    {
        xPosition = 0;
        currentDx *= -1;
    }
    //x값과 가로 길이 더한게 창 넘어가면 창에서 글자 길이 빼서 클램프 해주고 방향 반대 설정
    if (xPosition + width >= Engine::Get().GetWidth())
    {
        xPosition = Engine::Get().GetWidth() - width;
        currentDx *= -1;
    }
    //x값과 마찬가지
    if (yPosition < 0)
    {
        yPosition = 0;
        currentDy *= -1;
    }
    //Todo: x값이랑 똑같은데 현재 height는 1이다, 나중에 2차원 액터 사용시 수정 필요
    if (yPosition + height >= Engine::Get().GetHeight())
    {
        yPosition = Engine::Get().GetHeight() - height;
        currentDy *= -1;
    }


}
