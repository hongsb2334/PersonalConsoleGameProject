#include "Enemy.h"
#include <Engine/Engine.h>
#include <Level/Level.h>
#include <Actor/Player.h>
#include <Actor/Obstacle.h>
#include <Actor/EnemyProjectile.h>
#include <cmath>
using namespace Craft;

Enemy::Enemy(const Craft::Vector2& position, int maxHp, int damage) : Damageable("enemy", position, Color::Red, maxHp), damage(damage), xPosition(static_cast<float>(position.x)), yPosition(static_cast<float>(position.y))
{
    fireTimer.SetTargetTime(fireInterval);
}

void Enemy::Tick(float deltaTime)
{
    super::Tick(deltaTime);

    Move(deltaTime);

    fireTimer.Tick(deltaTime);
    if (fireTimer.IsTimeOut())
    {
        Fire();
        fireTimer.Reset();
    }
}

void Enemy::OnCollision(const std::shared_ptr<Actor>& other)
{
    super::OnCollision(other);
    
    //만약 발사체가 Obstacle에 닿으면 이전 위치로 업데이트
    if (other->IsTypeOf<Obstacle>())
    {
        SetPosition(GetPreviousPosition());
        xPosition = static_cast<float>(previousPosition.x);
        yPosition = static_cast<float>(previousPosition.y);
    }

    ////적 안겹치게 처리하기 위해 이전 위치 업데이트
    //if (other->IsTypeOf<Enemy>())
    //{
    //    SetPosition(GetPreviousPosition());
    //    xPosition = static_cast<float>(previousPosition.x);
    //    yPosition = static_cast<float>(previousPosition.y);
    //}
}

void Enemy::Move(float deltaTime)
{
    float dX = 0;
    float dY = 0;
    MovingPattern(dX, dY);

    xPosition += dX * moveSpeed * deltaTime;
    yPosition += dY * moveSpeed * deltaTime;

    //Player와 같은 로직
    Vector2 newPosition = GetPosition();
    newPosition.x = static_cast<int>(xPosition);
    newPosition.y = static_cast<int>(yPosition);
    SetPosition(newPosition);
}

void Enemy::MovingPattern(float& outDx, float& outDy) const
{
    outDx = 0.0f;
    outDy = 0.0f;
}

void Enemy::Fire()
{
    Vector2 bulletPosition(GetPosition().x + (width / 2), GetPosition().y);

    std::shared_ptr<Level> owner = GetOwner();
    if(!owner)
    {
        return;
    }

    auto player = owner->FindActor<Player>();
    if (!player)
    {
        return;
    }


    //대각선 정규화 로직
    float dx = static_cast<float>(player->GetPosition().x - GetPosition().x);
    float dy = static_cast<float>(player->GetPosition().y - GetPosition().y);
    float length = std::sqrt(dx * dx + dy * dy);
    if (length > 0.0f)
    {
        dx /= length;
        dy /= length;
    }

    owner->SpawnActor<EnemyProjectile>(bulletPosition, dx, dy, damage);

}

void Enemy::OnDeath()
{
    super::OnDeath();
}

void Enemy::Clamp()
{
    //x값이 0보다 작아지면 0으로 클램프 후 부호 바꿔서 반대로 진행하게 함
    if (xPosition < 1.0f)
    {
        xPosition = 1.0f;
    }
    //x값과 가로 길이 더한게 창 넘어가면 창에서 글자 길이 빼서 클램프 해주고 방향 반대 설정
    if (xPosition + width > Engine::Get().GetWidth() - 1)
    {
        xPosition = Engine::Get().GetWidth() - width - 1;
    }
    //x값과 마찬가지
    if (yPosition < 1.0f)
    {
        yPosition = 1.0f;
    }
    //Todo: x값이랑 똑같은데 현재 height는 1이다, 나중에 2차원 액터 사용시 수정 필요
    if (yPosition + height > Engine::Get().GetHeight() - 1)
    {
        yPosition = Engine::Get().GetHeight() - height - 1;
    }
}
