#include "ChaserEnemy.h"
#include <Level/Level.h>
#include <Actor/Player.h>
#include <cmath>

using namespace Craft;

ChaserEnemy::ChaserEnemy(const Craft::Vector2& position, int maxHp, int damage) : Enemy(position, maxHp, damage)
{
    ChangeImage("CHASER");
}
void ChaserEnemy::MovingPattern(float& outDx, float& outDy) const
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

    //Enemy->Player 방향으로 다가오는 패턴
    outDx = static_cast<float>(player->GetPosition().x - GetPosition().x);
    outDy = static_cast<float>(player->GetPosition().y - GetPosition().y);

    float length = std::sqrt(outDx * outDx + outDy * outDy);
    if (length > 0.0f)
    {
        outDx /= length;
        outDy /= length;
    }
}
