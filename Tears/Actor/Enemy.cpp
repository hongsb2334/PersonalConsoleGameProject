#include "Enemy.h"
#include <Engine/Engine.h>
#include <Level/Level.h>
#include <Actor/Player.h>
#include <Actor/EnemyProjectile.h>
#include <cmath>
using namespace Craft;

Enemy::Enemy(const Craft::Vector2& position) : Damageable("enemy", position, Color::Red, 1), xPosition(static_cast<float>(position.x)), yPosition(static_cast<float>(position.y))
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

void Enemy::Move(float deltaTime)
{
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

    //정규화 로직
    float dx = static_cast<float>(player->GetPosition().x - GetPosition().x);
    float dy = static_cast<float>(player->GetPosition().y - GetPosition().y);
    float length = std::sqrt(dx * dx + dy * dy);
    if (length > 0.0f)
    {
        dx /= length;
        dy /= length;
    }

    xPosition += dx * moveSpeed * deltaTime;
    yPosition += dy * moveSpeed * deltaTime;

    //Player와 같은 로직
    Vector2 newPosition = GetPosition();
    newPosition.x = static_cast<int>(xPosition);
    newPosition.y = static_cast<int>(yPosition);
    SetPosition(newPosition);
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

    owner->SpawnActor<EnemyProjectile>(bulletPosition, dx, dy);

}

void Enemy::OnDeath()
{
    super::OnDeath();
}
