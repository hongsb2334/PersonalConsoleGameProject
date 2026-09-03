#include "Room.h"
#include <Actor/Player.h>
#include <Actor/HealItem.h>
#include <Engine/Engine.h>
#include <Render/Renderer.h>
#include <Util/Util.h>
using namespace Craft;

void Room::OnInitialized()
{
    super::OnInitialized();

    Engine::Get().PlayBackGroundMusic("BackGroundMusic.wav");

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

    //힐 아이템 스폰(5초마다 랜덤 위치에 스폰)
    healItemTimer.Tick(deltaTime);
    if (healItemTimer.IsTimeOut())
    {
        int x = Util::RandomRange(1, Engine::Get().GetWidth() - 2);
        int y = Util::RandomRange(1, Engine::Get().GetHeight() - 2);
        SpawnActor<HealItem>(Vector2(x, y));
        healItemTimer.Reset();
    }


    if (CountAliveEnemies() == 0)
    {
        isCleared = true;
        OnRoomCleared();
    }
}

void Room::Draw()
{
    super::Draw();

    std::shared_ptr<Player> player = FindActor<Player>();
    if (player)
    {
        std::string hpText = "HP : " + std::to_string(player->GetHp()) + " / " + std::to_string(player->GetMaxHp());
        Renderer::Get().Submit(hpText, Vector2(1, 0), Color::White);
    }
}

void Room::SpawnDoor()
{
    //Todo: 기존 Room3, Room4 등에서 사용하던 로직을 통합하기 위해 구현
}

void Room::SpawnEnemies()
{
    //Todo: SpawnEnemies를 별도 Room cpp들에서 구현했던걸 여기에 옮기고 시작방 스킵(노드 검사해서 startRoom이면 스킵하는 방식으로 구현)
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
