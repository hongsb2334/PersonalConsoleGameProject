#pragma once
#include <Actor/Damageable.h>
#include <Util/Timer.h> 

class Enemy : public Craft::Damageable
{
    TYPE_DECLARATIONS(Enemy, Damageable)

public:
    Enemy(const Craft::Vector2& position, int maxHp = 1, int damage = 1);
    virtual ~Enemy() = default;

protected:
    virtual void Tick(float deltaTime) override;
    //chaser 로직을 함수 따로 만들어서 그대로 옮기고
    //Move를 virtual이 아닌 공용 로직으로 만듬
    void Move(float deltaTime);
    //MovingPattern을 오버라이드 하는 방식으로 적의 여러 움직임 패턴을 구현
    virtual void MovingPattern(float& outDx, float& outDy) const;
    virtual void Fire();
    virtual void OnDeath() override;

    Timer fireTimer;
    float fireInterval = 1.0f;
    int damage = 1;

    float xPosition = 0.0f;
    float yPosition = 0.0f;
    float moveSpeed = 10.0f;

};

