#pragma once
#include <Engine/Engine.h>
#include <Util/Util.h>
#include <Level/Level.h>
#include <Actor/Enemy.h>
#include <Actor/Door.h>
#include <Actor/Obstacle.h>
#include <Core/RunState.h>
#include <Util/Timer.h>

#include <stack>
#include <cstdint>


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

    bool IsBlocked(int x, int y) const;

    bool IsAreaBlocked(int x, int y, int width, int height) const;
protected:
    //문 스폰하는 함수, 기존 Room3, Room4 등에서 사용하던 로직 통합 구현
    virtual void SpawnDoor();

    //적 스폰하는 함수, 이전에 랜덤한 초마다 스폰하던 것과 달리 한번에 스폰
    virtual void SpawnEnemies();

    //플레이어 스폰
    void SpawnPlayer();

    //obstacle 스폰
    void SpawnObstacles();

    //방 클리어시 호출(클리어 후 등록된 문 활성화)
    virtual void OnRoomCleared();
    
    virtual bool IsRoomCleared() const { return CountAliveEnemies() == 0; }

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
        std::shared_ptr<T> door = SpawnActor<T>(std::forward<Args>(args)...);
        doorList.push_back(door);
        return door;
    }

    //Obstacle을 스폰하고 추적하는 코드
    template <typename T, typename ...Args, typename = std::enable_if_t<std::is_base_of<Obstacle, T>::value>>
    std::shared_ptr<T> TrackSpawnedObstacle(Args&& ...args)
    {
        std::shared_ptr<T> obstacle = SpawnActor<T>(std::forward<Args>(args)...);
        obstacleList.push_back(obstacle);
        return obstacle;
    }

    //랜덤 스폰하는 함수
    template <typename T>
    void SpawnEnemyRandomly(int count)
    {
        for (int i = 0; i < count; i++)
        {
            int x = Util::RandomRange(1, Craft::Engine::Get().GetWidth() - 6);
            int y = Util::RandomRange(1, Craft::Engine::Get().GetHeight() - 6);
            //만약 적이 스폰될 위치가 obstacle의 위치라면 재추첨
            if (IsAreaBlocked(x, y, T::spawnWidth, 1)) { --i; continue; }
            TrackSpawnedEnemy<T>(Craft::Vector2(x, y));
        }
        
    }

    //방을 그리드로 만들기 (이동 가능하면 0, 불가능하면 1)
    void BuildRoomGrid();

    inline int Index(int x, int y) const { return y * gridW + x; }
    
    void DrawMiniMap();

protected:
    //Enemy의 수를 카운트하는 함수
    int CountAliveEnemies() const;

    //플레이어 액터의 사이즈를 받아 플레이어가 다음 레벨에 넘어갈 시 스폰할 위치 리턴 
    Craft::Vector2 GetEntryPosition(EntryDirection direction, int playerWidth, int playerHeight) const;

protected:
    //적 리스트
    std::vector<std::shared_ptr<Enemy>> spawnedEnemyList;
    //문 리스트
    std::vector<std::shared_ptr<Door>> doorList;
    //장애물 리스트
    std::vector<std::shared_ptr<Obstacle>> obstacleList;


    //방 내부의 그리드
    std::vector<uint8_t> roomGrid;
    int gridW = 0;
    int gridH = 0;
    



private:
    Timer healItemTimer{ 5.0f };

    
};