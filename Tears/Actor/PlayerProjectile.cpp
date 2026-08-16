#include "PlayerProjectile.h"
#include <Engine/Engine.h>
#include <Input/Input.h>
#include <Actor/Enemy.h>
#include <cmath>
using namespace Craft;

PlayerProjectile::PlayerProjectile(const Craft::Vector2& position) 
    : Actor("@", position, Color::Blue), xPosition(static_cast<float>(position.x)), yPosition(static_cast<float>(position.y))
{
    if (Input::Get().GetKey(VK_UP))
    {
        yDirection = -1.0f;
    }
    if (Input::Get().GetKey(VK_DOWN))
    {
        yDirection = 1.0f;
    }
    if (Input::Get().GetKey(VK_RIGHT))
    {
        xDirection = 1.0f;
    }
    if (Input::Get().GetKey(VK_LEFT))
    {
        xDirection = -1.0f;
    }

    //아무것도 안누르고 있으면
    if (xDirection == 0.0f && yDirection == 0.0f)
    {
        yDirection = -1.0f;
    }

    
}

void PlayerProjectile::Tick(float deltaTime)
{
    super::Tick(deltaTime);

    float length = std::sqrt(xDirection * xDirection + yDirection * yDirection);
    if (length > 0.0f)
    {
        xDirection /= length;
        yDirection /= length;
    }

    //8방향 이동 처리
    xPosition += xDirection * moveSpeed * deltaTime;
    yPosition += yDirection * moveSpeed * deltaTime;

    //좌표 검사 (화면 밖으로 벗어났는지 확인)
    if (yPosition <0.0f || xPosition <0.0f || xPosition >= Engine::Get().GetWidth() || yPosition >= Engine::Get().GetHeight())
    {
        //삭제 처리
        Destroy();
        return;
    }

    //위치 값 설정 및 갱신
    Vector2 newPosition = GetPosition();
    newPosition.x = static_cast<int>(xPosition);
    newPosition.y = static_cast<int>(yPosition);
    SetPosition(newPosition);
    
}

void PlayerProjectile::OnCollision(const std::shared_ptr<Craft::Actor>& other)
{
    //발사체가 Enemy에 닿으면 데미지를 주고 소멸
    if (other->IsTypeOf<Enemy>())
    {
        //사운드 효과 재생
        Engine::Get().PlayOneShot("Explosion.wav");

        Cast<Damageable>(other)->TakeDamage(damage);
        Destroy();
    }
}


