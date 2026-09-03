#include "Room.h"
#include "BossRoom.h"
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
    //RunState
    std::shared_ptr<RunState> runState = Engine::Get().GetGameInstance<RunState>();
    
    //현재 방 불러와서
    RoomNode* current = runState->currentRoom;
    RoomNode* bossRoom = runState->dungeonMap.GetBossRoom();
    if (!current)
    {
        return;
    }

    //door 위치 설정
    Vector2 topDoorPosition(Engine::Get().GetWidth() / 2 - (Door::doorLength / 2), 0);
    Vector2 bottomDoorPosition(Engine::Get().GetWidth() / 2 - (Door::doorLength / 2), Engine::Get().GetHeight() - 1);
    Vector2 rightDoorPosition(Engine::Get().GetWidth() - 1, Engine::Get().GetHeight() / 2 - (Door::doorLength / 2));
    Vector2 leftDoorPosition(1, Engine::Get().GetHeight() / 2 - (Door::doorLength / 2));

    std::vector<DoorInfo> doorInfo = {
        {EntryDirection::Top, current->topRoom, topDoorPosition, DoorDirection::Horizontal},
        {EntryDirection::Right, current->rightRoom, rightDoorPosition, DoorDirection::Vertical},
        {EntryDirection::Bottom, current->bottomRoom, bottomDoorPosition, DoorDirection::Horizontal},
        {EntryDirection::Left, current->leftRoom, leftDoorPosition, DoorDirection::Vertical}
    };

    
    for (const DoorInfo& info : doorInfo)
    {
        if (!info.neighbor || !info.neighbor->occupied)
        {
            continue;
        }
        EntryDirection entry = info.entryDirection;
        RoomNode* neighbor = info.neighbor;

        TrackSpawnedDoor<Door>(info.position, [runState, entry, neighbor, bossRoom]()
            {
                runState->entryDirection = GetOppositeDirection(entry);
                runState->currentRoom = neighbor;
                if (neighbor == bossRoom)
                {
                    Engine::Get().AddNewLevel<BossRoom>();
                }
                else
                {
                    Engine::Get().AddNewLevel<Room>();
                }
            },
            info.doorDirection
        );

    }    
    
}

void Room::SpawnEnemies()
{
    //현재 노드와 시작 룸 불러오기
    RoomNode* current = Engine::Get().GetGameInstance<RunState>()->currentRoom;
    RoomNode* start = Engine::Get().GetGameInstance<RunState>()->dungeonMap.GetStartRoom();
    RoomNode* boss = Engine::Get().GetGameInstance<RunState>()->dungeonMap.GetBossRoom();
    //시작 룸일 경우 적 스폰이 필요없으므로 스킵
    if (current == start)
    {
        return;
    }
    //보스룸일 경우 일반 room과 다른 패턴의 적 사용
    else if (current == boss)
    {
        //Todo: boss방 적 스폰 및 패턴 설정
    }
    else
    {
        int enemyCount = Util::RandomRange(3, 7);

        for (int i = 0; i < enemyCount; i++)
        {
            //Todo: 화면 가장자리 짤리는 현상 일어날 수 있어 확인 필요
            int x = Util::RandomRange(1, Engine::Get().GetWidth() - 6);
            int y = Util::RandomRange(1, Engine::Get().GetHeight() - 6);

            TrackSpawnedEnemy<Enemy>(Vector2(x, y));
        }
    }
}

void Room::SpawnPlayer()
{
    //Player의 상태 가져오기
    std::shared_ptr<RunState> runState = Engine::Get().GetGameInstance<RunState>();
    
    //플레이어 스폰 후 위치 설정
    std::shared_ptr<Player> player = SpawnActor<Player>(runState->playerHp);
    //Todo: SpawnPosition이 쓰이는 지 확인 필요, 중복 코드 일지도 모름
    player->SetSpawnPosition(GetEntryPosition(runState->entryDirection, player->GetWidth(), player->GetHeight()));

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

Vector2 Room::GetEntryPosition(EntryDirection direction, int playerWidth, int playerHeight) const
{
    int halfWidth = playerWidth / 2;
    //Todo: 지금은 Player가 한 줄이라 halfHeight 의미가 없다
    int halfHeight = playerHeight / 2;

    switch (direction)
    {
    case EntryDirection::Top:
        return Vector2(Engine::Get().GetWidth() / 2 - halfWidth, 1);
    case EntryDirection::Bottom:
        return Vector2(Engine::Get().GetWidth() / 2 - halfWidth, Engine::Get().GetHeight() - playerHeight - 1);
    case EntryDirection::Left:
        return Vector2(1, Engine::Get().GetHeight() / 2 - halfHeight);
    case EntryDirection::Right:
        return Vector2(Engine::Get().GetWidth() - 2, Engine::Get().GetHeight() / 2 - halfHeight);
    case EntryDirection::None:
    default:
        //시작 방에서는 EntryDirection이 기본 None으로 설정되어 있어 default 까지 내려오게 설정, 이렇게 하면 시작 방에서 중앙에 스폰됨
        return Vector2(Engine::Get().GetWidth() / 2 - halfWidth, Engine::Get().GetHeight() / 2 - halfHeight);
    }
}
