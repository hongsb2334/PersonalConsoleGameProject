#define NOMINMAX
#include "Room.h"
#include "BossRoom.h"
#include "StartRoom.h"
#include <Actor/Enemy/ChaserEnemy.h>
#include <Actor/Enemy/WandererEnemy.h>
#include <Actor/Enemy/OrbiterEnemy.h>
#include <Actor/Enemy/AStarEnemy.h>
#include <Actor/Enemy/ChargerEnemy.h>
#include <Actor/Player.h>
#include <Actor/HealItem.h>
#include <Engine/Engine.h>
#include <Render/Renderer.h>
#include <Util/Util.h>
#include <Input/Input.h>
#include <Actor/Obstacle.h>
#include <Windows.h>
#include <algorithm>

using namespace Craft;
//토글로 설정한 것들
//못가는 곳 보여주는 토글
static bool showRoomGrid = false;
//A* 경로 보여주는 토글
static bool showPath = false;
//fps on/off 토글
static bool showFps = false;
//맵 전체 보여주기 on/off 토글
static bool showMap = false;

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
    
    //이동 불가능한 장애물 및 벽 표시 토글 키
    if (Input::Get().GetKeydown('G')) showRoomGrid = !showRoomGrid;
    //A* 시각화 토글 키
    if (Input::Get().GetKeydown('P')) showPath = !showPath;
    //fps on/off 토글 키
    if (Input::Get().GetKeydown('F')) showFps= !showFps;
    //맵 on/off 토글 키
    if (Input::Get().GetKeydown('M')) showMap = !showMap;


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

        if (!IsBlocked(x, y))
        {
            SpawnActor<HealItem>(Vector2(x, y));
        }
        
        healItemTimer.Reset();
    }


    if (IsRoomCleared())
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

    float fps = 1.0f / Engine::Get().GetDeltaTime();

    std::string fpsText = "fps : " + std::to_string(fps);
    if (showFps)
    {
        Renderer::Get().Submit(fpsText, Vector2(1, Engine::Get().GetHeight() - 1), Color::White);
    }
    

    //A* 시각화 
    for (std::shared_ptr<Enemy> enemy : spawnedEnemyList)
    {
        //AStarEnemy와 Player 경로 시각화
        std::shared_ptr<AStarEnemy> astarEnemy = Cast<AStarEnemy>(enemy);
        if (astarEnemy && astarEnemy->IsActive() && showPath)
        {
            for(Craft::Vector2& position : astarEnemy->GetCurrentPath())
            Renderer::Get().Submit(std::string("#"), position, Color::Green);
        }
    }

    //미니맵 그리기
    DrawMiniMap();
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
    Vector2 topDoorPosition(Engine::Get().GetWidth() / 2 - (Door::doorLength / 2), 1);
    Vector2 bottomDoorPosition(Engine::Get().GetWidth() / 2 - (Door::doorLength / 2), Engine::Get().GetHeight() - 2);
    Vector2 rightDoorPosition(Engine::Get().GetWidth() - 2, Engine::Get().GetHeight() / 2 - (Door::doorLength / 2));
    Vector2 leftDoorPosition(1, Engine::Get().GetHeight() / 2 - (Door::doorLength / 2));

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

        std::shared_ptr<Door> door = TrackSpawnedDoor<Door>(info.position, [runState, entry, neighbor, bossRoom, startRoom]()
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

        if (info.neighbor == bossRoom)
        {
            door->SetColor(Color::Red);
        }

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
    else
    {
        //Orbiter, Wanderer, AstarEnemy만 일반 룸에 소환, (보스룸에는 ChargeEnemy까지 소환)
        int orbiterEnemyCount = Util::RandomRange(1, 2);
        int wandererEnemyCount = Util::RandomRange(1, 2);
        int AStarEnemyCount = Util::RandomRange(1, 2);
        
        SpawnEnemyRandomly<OrbiterEnemy>(orbiterEnemyCount);
        SpawnEnemyRandomly<WandererEnemy>(wandererEnemyCount);
        SpawnEnemyRandomly<AStarEnemy>(AStarEnemyCount);
        
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

   
    //방이 생성될 때마다 시드 멤버를 가지게 해서 장애물이 설치되는 위치와 갯수 설정. 이렇게 하면 방의 장애물의 위치와 갯수가 유지되어 다른 방에 갔다가 재입장해도 그대로 남아있다.
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

                roomGrid[Index(x, y)] = 1;
            }
        }
    }
}

void Room::DrawMiniMap()
{
    std::shared_ptr<RunState> runState = Engine::Get().GetGameInstance<RunState>();
    //너무길어서 auto씀
    const auto& dungeon = runState->dungeonMap.GetDungeon();
    RoomNode* bossRoom = runState->dungeonMap.GetBossRoom();
    RoomNode* currentRoom = runState->currentRoom;
    
    int minX = 0;
    int maxX = 0;
    int minY = 0;
    int maxY = 0;
    
    for (const auto& pair : dungeon)
    {
        //방 정보
        const RoomNode& node = pair.second;
        if (!node.occupied)
        {
            continue;
        }
        
        minX = std::min(minX, node.coord.x);
        maxX = std::max(maxX, node.coord.x);

        minY = std::min(minY, node.coord.y);
        maxY = std::max(maxY, node.coord.y);
    }
    
    int originX = Engine::Get().GetWidth() - (maxX - minX + 1) - 1;
    int originY = 1;


    for (const auto& pair : dungeon)
    {
        //방 정보
        const RoomNode& node = pair.second;
        //방이 있는데 완전히 생성된 방이 아니면
        if (!node.occupied)
        {
            continue;
        }

        //안가본 방은 안그린다.
        if (!node.isCleared && &node != currentRoom && !showMap)
        {
            continue;
        }


        Vector2 position = Vector2(originX + (node.coord.x - minX), originY + (node.coord.y - minY));

        if (&node == currentRoom)
        {
            Renderer::Get().Submit("#", position, Color::Blue);
        }
        else if (&node == bossRoom)
        {
            Renderer::Get().Submit("#", position, Color::Red);
        }
        else if (node.isCleared)
        {
            Renderer::Get().Submit("#", position, Color::Green);
        }
        else Renderer::Get().Submit("#", position, Color::White);
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

bool Room::IsAreaBlocked(int x, int y, int width, int height) const
{
    for (int i = x; i < x + width; i++)
    {
        if (IsBlocked(i, y))
        {
            return true;
        }
    }
    return false;
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
        return Vector2(Engine::Get().GetWidth() / 2 - halfWidth, 2);
    case EntryDirection::Bottom:
        return Vector2(Engine::Get().GetWidth() / 2 - halfWidth, Engine::Get().GetHeight() - playerHeight - 2);
    case EntryDirection::Left:
        return Vector2(2, Engine::Get().GetHeight() / 2 - halfHeight);
    case EntryDirection::Right:
        return Vector2(Engine::Get().GetWidth() - playerWidth - 2, Engine::Get().GetHeight() / 2 - halfHeight);
    case EntryDirection::None:
    default:
        //시작 방에서는 EntryDirection이 기본 None으로 설정되어 있어 default 까지 내려오게 설정, 이렇게 하면 시작 방에서 중앙에 스폰됨
        return Vector2(Engine::Get().GetWidth() / 2 - halfWidth, Engine::Get().GetHeight() / 2 - halfHeight);
    }
}
