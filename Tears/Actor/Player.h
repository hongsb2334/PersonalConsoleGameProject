#pragma once
#include <Actor/Damageable.h>
#include <Util/Timer.h>

// wasd 이동/스페이스 키로 탄약을 발사하는 플레이어
//Player는 액터를 상속하는 Damageable을 상속한다.
class Player : public Craft::Damageable
{
    //발사 모드 (단발/연발)
    enum class FireMode
    {
        None = -1,
        OneShot,
        Repeat
    };
    //커스텀 타입 등록
    TYPE_DECLARATIONS(Player, Damageable)

public:
    Player();

private:
    virtual void OnDeath() override;

    // 이벤트 함수 오버라이드
    virtual void Tick(float deltaTime) override;

    //충돌 이벤트 함수 오버라이드
    virtual void OnCollision(const std::shared_ptr<Actor>& other) override;

    //이동 처리 함수
    void Move(float xDirection, float yDirection, float deltaTime);

    //탄약 발사 함수
    void Fire();

    //연사 함수
    void FireInterval();

    //발사 가능 여부 확인 함수
    inline bool CanShoot() const { return timer.IsTimeOut(); }


private:
    //이동 처리에 필요한 변수
    float xPosition = 0.0f;

    float yPosition = 0.0f;

    //이동 속도 변수
    float moveSpeed = 30.0f;

    //발사 모드 추가
    FireMode fireMode = FireMode::None;

    //타이머 변수
    Timer timer;

    //연사 시 발사 간격(단위: 초)
    float fireInterval = 0.3f;

    

};