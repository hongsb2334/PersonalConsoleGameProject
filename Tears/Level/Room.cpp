#include "Room.h"
#include <Actor/Player.h>
#include <Engine/Engine.h>

using namespace Craft;

void Room::OnInitialized()
{
    super::OnInitialized();

    SpawnPlayer();
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

void Room::SpawnPlayer()
{
    //Player의 상태 가져오기
    std::shared_ptr<RunState> runState = Engine::Get().GetGameInstance<RunState>();
    
    //플레이어 스폰 후 위치 설정
    std::shared_ptr<Player> player = SpawnActor<Player>(runState->playerHp);
    //Todo: SpawnPosition이 쓰이는 지 확인 필요, 중복 코드 일지도 모름
    player->SetSpawnPosition(GetEntryPosition(runState->entryDirection, player->GetWidth()));

    //한 방에서 사용한 진입 위치 값은 다음 방에서 사용하기 위해 초기화
    runState->entryDirection = EntryDirection::None;
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

Vector2 Room::GetEntryPosition(EntryDirection direction, int playerWidth) const
{
    int half = playerWidth / 2;

    switch (direction)
    {
    case EntryDirection::Top:
        return Vector2(Engine::Get().GetWidth() / 2 - half, 1);
    case EntryDirection::Bottom:
        return Vector2(Engine::Get().GetWidth() / 2 - half, Engine::Get().GetHeight() - 2);
    case EntryDirection::Left:
        return Vector2(1, Engine::Get().GetHeight() / 2);
    case EntryDirection::Right:
        return Vector2(Engine::Get().GetWidth() - 2, Engine::Get().GetHeight() / 2);
    default:
        return Vector2(Engine::Get().GetWidth() / 2 - half, Engine::Get().GetHeight() - 2);
    }
}
