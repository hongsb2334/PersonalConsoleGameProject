#include "Room.h"
using namespace Craft;

void Room::OnInitialized()
{
    super::OnInitialized();

    SpawnDoor();
    SpawnEnemies();

    //시작 방은 적이 없으므로 바로 클리어 처리를 해서 문을 연다
    if (spawnedEnemyList.empty())
    {
        isCleared = true;
        OnRoomCleared();
    }

}

void Room::Tick(float deltaTime)
{
    super::Tick(deltaTime);
    
    //기본 클리어 플래그 false로 시작해서 밑의 if문에서 플래그가 true로 바뀌면 리턴하여 판정 로직 반복안되게 하는 코드
    if (isCleared)
    {
        return;
    }

    if (CountAliveEnemies() == 0)
    {
        isCleared = true;
        OnRoomCleared();
    }
}

void Room::OnRoomCleared()
{
    for (std::shared_ptr<Door>& door : doorList)
    {
        if (door)
        {
            door->Open();
        }
    }
}

int Room::CountAliveEnemies() const
{
    int count = 0;
    for (const std::shared_ptr<Enemy>& elem : spawnedEnemyList)
    {
        if (elem->IsActive())
        {
            count++;
        }
    }
    return count;
}
