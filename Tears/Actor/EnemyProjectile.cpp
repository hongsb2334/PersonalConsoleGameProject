#include "EnemyProjectile.h"
#include <Engine/Engine.h>
#include <Actor/Player.h>
#include <Actor/Damageable.h>

using namespace Craft;

EnemyProjectile::EnemyProjectile(const Craft::Vector2& position, float xDirection, float yDirection, int damage)
    : Actor("*", position, Color::Yellow), 
    xPosition(static_cast<float>(position.x)), yPosition(static_cast<float>(position.y)), 
    xDirection(xDirection), yDirection(yDirection), damage(damage)
{
}

void EnemyProjectile::Tick(float deltaTime)
{
    super::Tick(deltaTime);

    xPosition += xDirection * moveSpeed * deltaTime;
    yPosition += yDirection * moveSpeed * deltaTime;

    if (xPosition < 0.0f || xPosition >= Engine::Get().GetWidth() || yPosition < 0.0f || yPosition >= Engine::Get().GetHeight())
    {
        Destroy();
        return;
    }

    Vector2 newPosition = GetPosition();
    newPosition.x = static_cast<int>(xPosition);
    newPosition.y = static_cast<int>(yPosition);
    SetPosition(newPosition);
}

void EnemyProjectile::OnCollision(const std::shared_ptr<Craft::Actor>& other)
{
    //발사체가 만약 Player에 닿으면 데미지를 주고 소멸
    if (other->IsTypeOf<Player>())
    {
        Engine::Get().PlayOneShot("Explosion.wav");
        Cast<Damageable>(other)->TakeDamage(damage);
        Destroy();
    }
}
