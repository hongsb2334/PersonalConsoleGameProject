#include "ChargerEnemy.h"
#include <Actor/Player.h>
#include <Engine/Engine.h>
#include <Level/Level.h>
#include <cmath>
using namespace Craft;
ChargerEnemy::ChargerEnemy(const Craft::Vector2& position, int maxHp, int damage) : Enemy(position, maxHp, damage), dashDirX(0.0f), dashDirY(0.0f)
{
    ChangeImage("Charger");
    
    baseSpeed = moveSpeed;
    timer.SetTargetTime(idleTime);
}

void ChargerEnemy::MovingPattern(float& outDx, float& outDy) const
{
    outDx = 0;
    outDy = 0;
    if (currentState == Dash)
    {
        outDx = dashDirX;
        outDy = dashDirY;
    }
}

void ChargerEnemy::Tick(float deltaTime)
{
    super::Tick(deltaTime);
    timer.Tick(deltaTime);

    if (timer.IsTimeOut())
    {
        switch (currentState)
        {
        case ChargerEnemy::Idle:
        {
            std::shared_ptr<Level> owner = GetOwner();
            if (!owner) break;
            std::shared_ptr<Player> player = owner->FindActor<Player>();
            if (!player) break;

            float dx = player->GetPosition().x - GetPosition().x;
            float dy = player->GetPosition().y - GetPosition().y;

            float length = std::sqrt(dx * dx + dy * dy);
            if (length > 0)
            {
                dashDirX = dx / length;
                dashDirY = dy / length;
            }

            currentState = Ready;
            color = Color::White;
            timer.SetTargetTime(readyTime);
            break;
        }
        case ChargerEnemy::Ready:
        {
            currentState = Dash;
            color = Color::Red;
            moveSpeed = baseSpeed * dashMultiplier;
            timer.SetTargetTime(dashTime);
            break;
        }
        case ChargerEnemy::Dash:
        {
            currentState = Idle;
            color = Color::Red;
            moveSpeed = baseSpeed;
            timer.SetTargetTime(idleTime);
            break;
        }
        default:
            break;
        }
        timer.Reset();
    }

    //벽 클램프 처리를 위한 변수
    bool hitWall = false;

    //x값이 0보다 작아지면 0으로 클램프 후 부호 바꿔서 반대로 진행하게 함
    if (xPosition < 0)
    {
        xPosition = 0.0f;
        hitWall = true;
    }
    //x값과 가로 길이 더한게 창 넘어가면 창에서 글자 길이 빼서 클램프 해주고 방향 반대 설정
    if (xPosition + width >= Engine::Get().GetWidth())
    {
        xPosition = Engine::Get().GetWidth() - width;
        hitWall = true;
    }
    //x값과 마찬가지
    if (yPosition < 0)
    {
        yPosition = 0;
        hitWall = true;
    }
    //Todo: x값이랑 똑같은데 현재 height는 1이다, 나중에 2차원 액터 사용시 수정 필요
    if (yPosition + height >= Engine::Get().GetHeight())
    {
        yPosition = Engine::Get().GetHeight() - height;
        hitWall = true;
    }

    //Todo: 클램프 직후 xPosition과 액터의 실제 위치가 한프레임 어긋나 있음(실제로 눈에 안보임). 나중에 문제 생기면 이 코드 활성화  
    Vector2 clamp = GetPosition();
    clamp.x = static_cast<int>(xPosition);
    clamp.y = static_cast<int>(yPosition);
    SetPosition(clamp);

    if (hitWall && currentState == Dash)
    {
        currentState = Idle;
        moveSpeed = baseSpeed;
        timer.SetTargetTime(idleTime);
        timer.Reset();
    }   
}

void ChargerEnemy::Fire()
{
    //chargerEnemy는 발사 안할거라 빈 함수 오버라이드
}
