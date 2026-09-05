#pragma once
#include "Enemy.h"
class OrbiterEnemy : public Enemy
{
    TYPE_DECLARATIONS(OrbiterEnemy, Enemy)

public:
    //선회형 적 (플레이어와 일정 거리를 유지하면서 원형으로 도는 적, 돌진 x)
    
    OrbiterEnemy(const Craft::Vector2& position, int maxHp = 2, int damage = 2);

    virtual void MovingPattern(float& outDx, float& outDy) const override;
    virtual void Tick(float deltaTime) override;

private:
    //유지하려는 거리
    float radius = 12.0f;

    //얼마나 원에 가깝게 도는지 (0.05 ~ 0.2 사이가 적당)
    float orbitStat = 0.1f;

    
    
};

