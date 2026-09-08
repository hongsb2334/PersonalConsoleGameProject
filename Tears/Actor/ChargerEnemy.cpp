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
    hitCoolDown.SetTargetTime(0.0f);
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
    hitCoolDown.Tick(deltaTime);

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

    //x값이 1보다 작아지면 1로 클램프
    if (xPosition < 1.0f)
    {
        xPosition = 1.0f;
        hitWall = true;
    }
    //x값과 가로 길이 더한게 창 넘어가면 창에서 글자 길이 빼서 클램프
    if (xPosition + width > Engine::Get().GetWidth() - 1)
    {
        xPosition = Engine::Get().GetWidth() - width - 1;
        hitWall = true;
    }
    //x값과 마찬가지
    if (yPosition < 1.0f)
    {
        yPosition = 1.0f;
        hitWall = true;
    }
    //Todo: x값이랑 똑같은데 현재 height는 1이다, 나중에 2차원 액터 사용시 수정 필요
    if (yPosition + height > Engine::Get().GetHeight() - 1)
    {
        yPosition = Engine::Get().GetHeight() - height - 1;
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

void ChargerEnemy::OnCollision(const std::shared_ptr<Craft::Actor>& other)
{
    super::OnCollision(other);
    //대시 상태가 아니면 데미지 x
    if (currentState != Dash) return;
    //중복 데미지를 막기 위해 쿨다운 상태가 아니면 리턴
    if (!hitCoolDown.IsTimeOut()) return;

    if (auto player = Cast<Player>(other))
    {
        player->TakeDamage(2);
        hitCoolDown.SetTargetTime(0.5f);
        hitCoolDown.Reset();
    }

}

void ChargerEnemy::Fire()
{
    //chargerEnemy는 발사 안할거라 빈 함수 오버라이드
}
