#pragma once
#include "Enemy.h"
class ChargerEnemy : public Enemy
{
    TYPE_DECLARATIONS(ChargerEnemy, Enemy)

public:
    ChargerEnemy(const Craft::Vector2& position, int maxHp = 2, int damage = 2);

    virtual void MovingPattern(float& outDx, float& outDy) const override;
    virtual void Tick(float deltaTime) override;

    //돌진형 적은 발사를 안할거라서 빈 함수로 오버라이드
    virtual void Fire() override;

    //돌진형 적의 상태 구성
    enum state
    {
        Idle,
        Ready,
        Dash
    };



private:
    Timer timer;

    //돌진할 방향
    float dashDirX;
    float dashDirY;

    //moveSpeed 백업용
    float baseSpeed;
    
    //상태 변수
    state currentState = Idle;

    //각 상태들 시간 및 속도 곱할 변수
    float idleTime = 1.2f;
    float readyTime = 0.5f;
    float dashTime = 0.6f;
    float dashMultiplier = 3.5f;

    

};

