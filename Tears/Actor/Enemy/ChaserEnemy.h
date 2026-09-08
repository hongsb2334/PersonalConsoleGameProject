#pragma once
#include "Enemy.h"
class ChaserEnemy : public Enemy
{
    TYPE_DECLARATIONS(ChaserEnemy, Enemy)

public:

    ChaserEnemy(const Craft::Vector2& position, int maxHp = 1, int damage = 1);

    virtual void MovingPattern(float& outDx, float& outDy) const override;
};

