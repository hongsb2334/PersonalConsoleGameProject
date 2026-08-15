#pragma once
#include <Actor/Actor.h>
class EnemyProjectile : public Craft::Actor
{
    TYPE_DECLARATIONS(EnemyProjectile, Actor);

public:
    //Vector2는 int형만 받아서, 나중에 정규화된 방향(float 값)을 넣어버리면 실수 값이 잘려 0이 된다, 따라서 EnemyProjectile은 방향을 float 값으로 따로 받는다.
    EnemyProjectile(
        const Craft::Vector2& position,
        float xDirection,
        float yDirection,
        int damage = 1
    );

private:
    virtual void Tick(float deltaTime) override;
    virtual void OnCollision(const std::shared_ptr<Craft::Actor>& other) override;

    float moveSpeed = 20.0f;

    float xPosition = 0.0f;
    float yPosition = 0.0f;
    float xDirection = 0.0f;
    float yDirection = 0.0f;

    int damage = 1;


};

