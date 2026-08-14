#pragma once
#include <Actor/Actor.h>
#include <functional>
//문의 방향(상하는 가로, 좌우는 세로 방향으로 배치한다.)
enum class DoorDirection
{
    Horizontal,
    Vertical
};

class Door : public Craft::Actor
{
    TYPE_DECLARATIONS(Door, Actor)

public:
    Door(const Craft::Vector2& position, std::function<void()> onEnter, DoorDirection doorDirection = DoorDirection::Horizontal);
    void Open();

protected:
    //적이 모두 처치되기 전에는 그려지지 않게 오버라이드
    virtual void Draw() override;
    virtual void OnCollision(const std::shared_ptr<Craft::Actor>& other);
private:
    
    bool isOpen = false;
    
    DoorDirection doorDirection = DoorDirection::Horizontal;

    //문에 들어갔을 때 할 동작(다음 Room으로 전환하는 로직을 람다로 전달 할거임)
    std::function<void()> onEnter;
};

