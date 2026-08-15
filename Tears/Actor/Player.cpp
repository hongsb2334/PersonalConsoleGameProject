#include "Player.h"
#include <Engine/Engine.h>
#include <Input/Input.h>
#include <Level/Level.h>
#include <Level/GameOverLevel.h>
#include <Actor/PlayerProjectile.h>
#include <cmath>


using namespace Craft;


Player::Player(int initialHp) : Damageable("ISSAC", Vector2::Zero, Color::Green, 10, initialHp), fireMode(FireMode::OneShot)
{
    //생성 위치 설정
    int x = (Engine::Get().GetWidth() / 2) - (width / 2);
    int y = (Engine::Get().GetHeight() - 2);
    SetPosition(Vector2(x, y));

    //x 위치 저장
    xPosition = static_cast<float>(x);
    yPosition = static_cast<float>(y);

    //연사 타이머 시간 설정
    timer.SetTargetTime(fireInterval);

}
void Player::SetSpawnPosition(const Craft::Vector2& newPosition)
{
    SetPosition(newPosition);
    xPosition = static_cast<float>(newPosition.x);
    yPosition = static_cast<float>(newPosition.y);
}
//죽음 함수 오버라이드
void Player::OnDeath()
{
    super::OnDeath();
    Engine::Get().AddNewLevel<GameOverLevel>();
}
// 이벤트 함수 오버라이드

void Player::Tick(float deltaTime)
{
    super::Tick(deltaTime);

    //ESC 종료 처리
    if (Input::Get().GetKeydown(VK_ESCAPE))
    {
        QuitGame();
    }

    //방향키 입력에 따른 이동 방향 설정
    //오른쪽 방향 : 1, 왼쪽 방향 : -1.
    float xDirection = 0.0f;
    if (Input::Get().GetKey('D'))
    {
        xDirection = 1.0f;
    }
    if (Input::Get().GetKey('A'))
    {
        xDirection = -1.0f;
    }

    float yDirection = 0.0f;
    if (Input::Get().GetKey('S'))
    {
        yDirection = 1.0f;
    }
    if (Input::Get().GetKey('W'))
    {
        yDirection = -1.0f;
    }

    //이동 함수 호출
    Move(xDirection, yDirection, deltaTime);

    ////발사 타이머 업데이트
    timer.Tick(deltaTime);

    if (fireMode == FireMode::OneShot)
    {
        //탄약 발사 처리
        if (Input::Get().GetKeydown(VK_SPACE))
        {
            Fire();
        }
    }
    //연사 모드 처리
    else if (fireMode == FireMode::Repeat)
    {
        if (Input::Get().GetKey(VK_SPACE))
        {
            //연사 발사 함수 호출
            FireInterval();
        }
    }

    //발사 모드 전환 처리
    if (Input::Get().GetKeydown('R'))
    {
        if (fireMode == FireMode::OneShot)
        {
            fireMode = FireMode::Repeat;
        }
        else if (fireMode == FireMode::Repeat)
        {
            fireMode = FireMode::OneShot;
        }
    }
}

void Player::OnCollision(const std::shared_ptr<Actor>& other)
{
    super::OnCollision(other);

    ////부딪힌 액터가 적 탄약이면 처리
    //if (other->IsTypeOf<EnemyBullet>())
    //{
    //    //플레이어 제거
    //    Destroy();

    //    //적 탄약 제거
    //    other->Destroy();

    //    //파괴 이펙트 생성
    //    if (GetOwner())
    //    {
    //        GetOwner()->SpawnActor<DestroyEffect>(GetPosition());
    //        //게임 오버(게임 종료)
    //        QuitGame();
    //    }
    //}
}


void Player::Move(float xDirection, float yDirection,float deltaTime)
{
    //대각 입력시 벡터 크기가 루트2가 되어 속도가 빨라지는 현상을 정규화로 해결
    //Todo: 대각 입력시 약간 잔상이 남는 현상(Render 문제인지 확인해야함)
    float length = std::sqrt(xDirection * xDirection + yDirection * yDirection);
    if (length > 0.0f)
    {
        xDirection /= length;
        yDirection /= length;
    }

    //x위치 업데이트
    //이동 처리->이동 방향과 빠르기를 적용해서 새로운 위치를 구하는 것
    //이동 방향(direction) / 빠르기(moveSpeed) / 시간
    //등속도 운동 : 이동 거리 = 기존 위치 + 이동 방향 * 빠르기 * 시간
    xPosition += xDirection * moveSpeed * deltaTime;
    yPosition += yDirection * moveSpeed * deltaTime;

    //화면 왼쪽 벗어나지 않도록 처리
    if (xPosition < 0)
    {
        xPosition = 0.0f;
    }

    //화면 위 벗어나지 않도록 처리
    if (yPosition < 0)
    {
        yPosition = 0.0f;
    }

    //화면 오른쪽 벗어나지 않도록 처리
    if (xPosition + width >= Engine::Get().GetWidth())
    {
        xPosition = static_cast<float>(Engine::Get().GetWidth() - width);
    }

    //화면 아래 벗어나지 않도록 처리
    //Todo: 만약 상하좌우 끝에 문 설치후 닿으면 다음 방으로 넘어갈 시 여기에 문제 있을 수도 있음
    if (yPosition + height >= Engine::Get().GetHeight())
    {
        yPosition = static_cast<float>(Engine::Get().GetHeight() - height);
    }

    //위치 업데이트
    Vector2 newPosition = GetPosition();

    //float 값을 int 값으로 형변환할 때 소숫점 값은 버림 처리됨.
    newPosition.x = static_cast<int>(xPosition);
    newPosition.y = static_cast<int>(yPosition);
    SetPosition(newPosition);
}

void Player::Fire()
{
    //탄약 생성 위치 구하기
    //플레이어의 가운데 위치

    Vector2 bulletPosition(GetPosition().x + (width / 2), GetPosition().y); // <=A=> 가 있으면 <가 GetPosition.x 위치고 width / 2 한걸 더하면 A가 가운데가 된다.

    //탄약 생성
    std::shared_ptr<Level> owner = GetOwner();
    if (owner)
    {
        owner->SpawnActor<PlayerProjectile>(bulletPosition);
    }
}

void Player::FireInterval()
{
    //발사 가능 여부 확인
    if (!CanShoot())
    {
        return;
    }

    //발사 처리
    Fire();

    //경과 시간 초기화
    timer.Reset();
}



