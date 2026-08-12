#pragma once
#include <Actor/Damageable.h>
#include <Util/Timer.h> 

class Enemy : public Craft::Damageable
{
    TYPE_DECLARATIONS(Enemy, Damageable)

public:
    Enemy(const Craft::Vector2& position);
    
protected:
    virtual void Tick(float deltaTime) override;
    virtual void Move(float deltaTime);
    virtual void Fire();
    virtual void OnDeath() override;

    Timer fireTimer;
    float fireInterval = 1.0f;
    int damage = 1;

    float xPosition = 0.0f;
    float yPosition = 0.0f;
    float moveSpeed = 20.0f;

};

