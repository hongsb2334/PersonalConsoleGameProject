#pragma once
#include "Enemy.h"
#include <Util/Timer.h>

class WandererEnemy : public Enemy
{
    TYPE_DECLARATIONS(WandererEnemy, Enemy)

public:
    //배회형 적 (플레이어의 위치에 상관 없이, 일정한 방향으로 방을 돌아다닌다. hp = 3, damage = 2로 설정)
    WandererEnemy(const Craft::Vector2& position, int maxHp = 3, int damage = 2);

    virtual void MovingPattern(float& outDx, float& outDy) const override;
    virtual void Tick(float deltaTime) override;

    

private:
    //현재 이동중인 방향
    float currentDx;
    float currentDy;

    Timer timer;
    

};

