#include "Room.h"
#include "BossRoom.h"
#include "StartRoom.h"
#include <Actor/ChaserEnemy.h>
#include <Actor/WandererEnemy.h>
#include <Actor/Player.h>
#include <Actor/HealItem.h>
#include <Engine/Engine.h>
#include <Render/Renderer.h>
#include <Util/Util.h>
#include <Input/Input.h>
#include <Actor/Obstacle.h>
#include <Windows.h>
using namespace Craft;
static bool showRoomGrid = false;

void Room::OnInitialized()
{
    super::OnInitialized();
    
    Engine::Get().PlayBackGroundMusic("BackGroundMusic.wav");
    
    SpawnPlayer();
    SpawnDoor();
    SpawnObstacles();
    BuildRoomGrid();
    SpawnEnemies();


    //현재 노드 가져온다
    RoomNode* node = Engine::Get().GetGameInstance<RunState>()->currentRoom;
    if (!node)
    {
        return;
    }

    //시작 방은 적이 없으므로 바로 클리어 처리를 해서 문을 연다
    if (spawnedEnemyList.empty())
    {
        node->isCleared = true;
        OnRoomCleared();
    }

}

void Room::Tick(float deltaTime)
{
    super::Tick(deltaTime);

    //현재 노드 가져와서
    RoomNode* node = Engine::Get().GetGameInstance<RunState>()->currentRoom;
    if (!node)
    {
        return;
    }
    
    if (Input::Get().GetKeydown('G')) showRoomGrid = !showRoomGrid;

    //기본 클리어 플래그 false로 시작해서 플래그가 true로 바뀌면 리턴하여 판정 로직 반복안되게 하는 코드
    if (node->isCleared)
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
        node->isCleared = true;
        OnRoomCleared();
    }
}

void Room::Draw()
{
    super::Draw();

    if (showRoomGrid)
    {
        for (int y = 0; y < gridH; ++y)
        {
            for (int x = 0; x < gridW; ++x)
            {
                //테두리면
                if (roomGrid[Index(x, y)] != 0)
                {
                    Renderer::Get().Submit("#", Vector2(x, y), Color::Purple, 5);
                }
            }
        }
    }

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
    RoomNode* startRoom = runState->dungeonMap.GetStartRoom();

    if (!current)
    {
        return;
    }

    //door 위치 설정
    Vector2 topDoorPosition(Engine::Get().GetWidth() / 2 - (Door::doorLength / 2), 0);
    Vector2 bottomDoorPosition(Engine::Get().GetWidth() / 2 - (Door::doorLength / 2), Engine::Get().GetHeight() - 1);
    Vector2 rightDoorPosition(Engine::Get().GetWidth() - 1, Engine::Get().GetHeight() / 2 - (Door::doorLength / 2));
    Vector2 leftDoorPosition(0, Engine::Get().GetHeight() / 2 - (Door::doorLength / 2));

    std::vector<DoorInfo> doorInfo = {
        {EntryDirection::Top, current->topRoom, topDoorPosition, DoorDirection::Horizontal},
        {EntryDirection::Right, current->rightRoom, rightDoorPosition, DoorDirection::Vertical},
        {EntryDirection::Bottom, current->bottomRoom, bottomDoorPosition, DoorDirection::Horizontal},
        {EntryDirection::Left, current->leftRoom, leftDoorPosition, DoorDirection::Vertical}
    };

    
    for (const DoorInfo& info : doorInfo)
    {
        //연결된 노드가 없거나 완전히 생성되지 않은 방이면 건너뜀
        if (!info.neighbor || !info.neighbor->occupied)
        {
            continue;
        }
        EntryDirection entry = info.entryDirection;
        RoomNode* neighbor = info.neighbor;

        TrackSpawnedDoor<Door>(info.position, [runState, entry, neighbor, bossRoom, startRoom]()
            {
                runState->entryDirection = GetOppositeDirection(entry);
                runState->currentRoom = neighbor;
                //보스룸이면 보스룸 생성
                if (neighbor == bossRoom)
                {
                    Engine::Get().AddNewLevel<BossRoom>();
                }
                //시작룸이면 시작룸 생성
                else if (neighbor == startRoom)
                {
                    Engine::Get().AddNewLevel<StartRoom>();
                }
                //나머지는 일반 룸 생성
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
    if (current == start || current->isCleared )
    {
        return;
    }
    //보스룸일 경우 일반 room과 다른 패턴의 적 사용
    else if (current == boss)
    {
        int wandererEnemyCount = Util::RandomRange(0, 2);

        //Todo: boss방 적 스폰 및 패턴 설정
        for (int i = 0; i < wandererEnemyCount; i++)
        {
            //Todo: 화면 가장자리 짤리는 현상 일어날 수 있어 확인 필요
            int x = Util::RandomRange(1, Engine::Get().GetWidth() - 6);
            int y = Util::RandomRange(1, Engine::Get().GetHeight() - 6);
            //만약 적이 스폰될 위치가 
            if (IsBlocked(x, y)) { --i; continue; }

            TrackSpawnedEnemy<WandererEnemy>(Vector2(x, y));
        }
    }
    else
    {
        int chaserEnemyCount = Util::RandomRange(3, 7);
        
        for (int i = 0; i < chaserEnemyCount; i++)
        {
            //Todo: 화면 가장자리 짤리는 현상 일어날 수 있어 확인 필요
            int x = Util::RandomRange(1, Engine::Get().GetWidth() - 6);
            int y = Util::RandomRange(1, Engine::Get().GetHeight() - 6);
            //만약 적이 스폰될 위치가 obstacle의 위치라면 재추첨
            if (IsBlocked(x, y)) { --i; continue; }
            TrackSpawnedEnemy<ChaserEnemy>(Vector2(x, y));
            
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

void Room::SpawnObstacles()
{
    std::shared_ptr<RunState> runState = Engine::Get().GetGameInstance<RunState>();

    //현재 노드와 시작 룸 불러오기
    RoomNode* current = Engine::Get().GetGameInstance<RunState>()->currentRoom;
    RoomNode* start = Engine::Get().GetGameInstance<RunState>()->dungeonMap.GetStartRoom();
    RoomNode* boss = Engine::Get().GetGameInstance<RunState>()->dungeonMap.GetBossRoom();

    //시작 룸일 경우 obstacle 스폰이 필요없으므로 스킵
    if (!current || !start || !boss || current == start )
    {
        return;
    }

   

    std::mt19937 rng(current->roomSeed);

    auto randRange = [&rng](int low, int high)
        {
            return std::uniform_int_distribution<int>(low, high)(rng);
        };

    int width = Engine::Get().GetWidth();
    int height = Engine::Get().GetHeight();

    const int xMin = width / 4;
    const int xMax = width - width / 4;
    const int yMin = height / 4;
    const int yMax = height - height / 4;

    int count = randRange(3, 6);
    for (int i = 0; i < count; i++)
    {
        int tileW = randRange(3, 7);
        int tileH = randRange(3, 6);

        int ox = randRange(xMin, xMax - tileW);
        int oy = randRange(yMin, yMax - tileH);

        TrackSpawnedObstacle<Obstacle>(Vector2(ox, oy), tileW, tileH);
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

void Room::BuildRoomGrid()
{
    gridW = Engine::Get().GetWidth();
    gridH = Engine::Get().GetHeight();

    //콘솔 크기만큼 0 채우기
    roomGrid.assign(gridW * gridH, 0);

    for (int x = 0; x < gridW; ++x)
    {
        //테두리 윗 줄과 아랫줄 은 1넣음 (0은 이동 가능, 1은 이동 불가)
        roomGrid[Index(x, 0)] = 1;
        roomGrid[Index(x, gridH - 1)] = 1;
    }
    for (int y = 0; y < gridH; ++y)
    {
        roomGrid[Index(0, y)] = 1;
        roomGrid[Index(gridW - 1, y)] = 1;
    }

    for (const std::shared_ptr<Obstacle>& obstacle : obstacleList)
    {
        //장애물의 위치
        int ox = obstacle->GetPosition().x;
        int oy= obstacle->GetPosition().y;
        //장애물의 가로, 세로 길이
        int ow = obstacle->GetWidth();
        int oh = obstacle->GetHeight();

        for (int y = oy; y < oy + oh; ++y)
        {
            for (int x = ox; x < ox + ow; ++x)
            {
                //만약 테두리를 넘어가면 건너뜀.
                if (x < 0 || x >= gridW || y < 0 || y >= gridH) continue;
            }
        }
    }
}

bool Room::IsBlocked(int x, int y) const
{
    //범위 밖 넘어가면 true 리턴, 아니면 실제 격자가 이동 불가능한지 여부 리턴
    if (x < 0 || x >= gridW || y < 0 || y >= gridH)
    {
        return true;
    }
    return roomGrid[(Index(x, y))] != 0;
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
    //Todo: 지금은 PlayerHeight가 한 줄이라 halfHeight 의미가 없다.
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
        return Vector2(Engine::Get().GetWidth() - playerWidth - 1, Engine::Get().GetHeight() / 2 - halfHeight);
    case EntryDirection::None:
    default:
        //시작 방에서는 EntryDirection이 기본 None으로 설정되어 있어 default 까지 내려오게 설정, 이렇게 하면 시작 방에서 중앙에 스폰됨
        return Vector2(Engine::Get().GetWidth() / 2 - halfWidth, Engine::Get().GetHeight() / 2 - halfHeight);
    }
}
