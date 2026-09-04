#pragma once
#include <Level/Level.h>
#include <Actor/Enemy.h>
#include <Actor/Door.h>
#include <Core/RunState.h>
#include <Util/Timer.h>

class Room : public Craft::Level
{
    TYPE_DECLARATIONS(Room, Level);

public:
    virtual void OnInitialized();
    virtual void Tick(float deltaTime);
    virtual void Draw();

    struct DoorInfo
    {
        EntryDirection entryDirection;
        RoomNode* neighbor;
        Craft::Vector2 position;
        DoorDirection doorDirection;
    };

protected:
    //문 스폰하는 함수, 기존 Room3, Room4 등에서 사용하던 로직 통합 구현
    virtual void SpawnDoor();

    //적 스폰하는 함수, 이전에 랜덤한 초마다 스폰하던 것과 달리 한번에 스폰
    virtual void SpawnEnemies();

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

    //플레이어 액터의 사이즈를 받아 
    Craft::Vector2 GetEntryPosition(EntryDirection direction, int playerWidth, int playerHeight) const;

protected:
    std::vector<std::shared_ptr<Enemy>> spawnedEnemyList;
    std::vector<std::shared_ptr<Door>> doorList;


private:
    Timer healItemTimer{ 5.0f };


};