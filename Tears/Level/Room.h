#pragma once
#include <Level/Level.h>
#include <Actor/Enemy.h>
#include <Actor/Door.h>
#include <Core/RunState.h>
class Room : public Craft::Level
{
    TYPE_DECLARATIONS(Room, Level);

public:
    virtual void OnInitialized();
    virtual void Tick(float deltaTime);

protected:
    //문 스폰하는 함수, 구현은 파생 클래스에서 함
    virtual void SpawnDoor() {}

    //적 스폰하는 함수, 이전에 랜덤한 초마다 스폰하던 것과 달리 한번에 스폰, 구현은 파생 클래스에서 함
    virtual void SpawnEnemies() {}

    //플레이어 스폰
    void SpawnPlayer();

    //방 클리어시 호출(클리어 후 등록된 문 활성화)
    virtual void OnRoomCleared();
    
    //Enemy를 스폰하고 추적하는 코드
    template <typename T, typename ...Args, typename = std::enable_if_t<std::is_base_of<Enemy, T>::value>>
    std::shared_ptr<T> TrackSpawnedEnemy(Args&& ...args)
    {
        //spawnActor가 반환하는 shared_ptr을 Room이 직접 보관해서 나중에 이 Room 안에 스폰된 적이 얼마나 살아있는지 추적하기 위해 사용
        std::shared_ptr<T> enemy = SpawnActor<T>(std::forward<Args>(args)...);
        spawnedEnemyList.push_back(enemy);
        return enemy;
    }

    //Door를 스폰하고 추적하는 코드
    template <typename T, typename ...Args, typename = std::enable_if_t<std::is_base_of<Door, T>::value>>
    std::shared_ptr<T> TrackSpawnedDoor(Args&& ...args)
    {
        //spaw
        std::shared_ptr<T> door = SpawnActor<T>(std::forward<Args>(args)...);
        doorList.push_back(door);
        return door;
    }
    
protected:
    //Enemy의 수를 카운트하는 함수
    int CountAliveEnemies() const;

    Craft::Vector2 GetEntryPosition(EntryDirection direction, int playerWidth) const;

protected:
    std::vector<std::shared_ptr<Enemy>> spawnedEnemyList;
    std::vector<std::shared_ptr<Door>> doorList;
    //Room 안의 적을 모두 처치했는지 플래그
    bool isCleared = false;
};