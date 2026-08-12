#include "Damageable.h"


using namespace Craft;
Damageable::Damageable(const std::string& image, const Vector2& position, Color color, int maxHp)
    : Actor(image, position, color), hp(maxHp), maxHp(maxHp)
{

}
void Damageable::TakeDamage(int damage)
{
    if (IsDead())
    {
        return;
    }

    hp -= damage;

    if (IsDead())
    {
        hp = 0;
        OnDeath();
    }
}
void Damageable::OnDeath()
{
    Destroy();
}

