#pragma once
#include <Actor/Actor.h>
//플레이어 발사체 클래스
class PlayerProjectile : public Craft::Actor
{
    //커스텀 타입 지정
    TYPE_DECLARATIONS(PlayerProjectile, Actor)

public:
    PlayerProjectile(const Craft::Vector2& position);

private:
    //이벤트 함수 오버라이딩
    virtual void Tick(float deltaTime) override;

    //이동 속도 (빠르기 단위: 초)
    float moveSpeed = 30.0f;

    //위치 갱신을 할 때 사용할 변수

    float xPosition = 0.0f;
    float yPosition = 0.0f;

    //발사체 이동 방향
    float xDirection = 0.0f;
    float yDirection = 0.0f;

   
};

