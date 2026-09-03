#include "Dungeon.h"
#include <queue>
#include <Util/Util.h>
#include <algorithm>
bool DungeonMap::GenerateRoom(int roomNum, int maxRegenerateNum)
{
    return false;
}

RoomNode* DungeonMap::FindRoom(const GridCoord& coord)
{
    return nullptr;
}

GridCoord DungeonMap::GetOffset(EntryDirection entry)
{
    GridCoord offset;
    switch (entry)
    {
    case EntryDirection::None:
        break;
    case EntryDirection::Top:
        offset.y = -1;
        break;
    case EntryDirection::Bottom:
        offset.y = 1;
        break;
    case EntryDirection::Left:
        offset.x = -1;
        break;
    case EntryDirection::Right:
        offset.x = 1;
        break;
    default:
        break;
    }

    return offset;
}

bool DungeonMap::GenerateRoomOnce(int roomNum)
{
    //이전 데이터 지움
    Clear();
    //BFS로 방 생성 위해 큐 생성
    std::queue<RoomNode*> roomQueue;

    //시작방 생성, (0, 0)에서 시작
    this->startRoom = &dungeon[GridCoord(0, 0)];
    startRoom->occupied = true;
    startRoom->distance = 0;
    roomQueue.push(startRoom);
    roomCount = 1;

    RoomNode* current = startRoom;
    
    while (!roomQueue.empty())
    {
        //방이 최소 한번은 확장했는지 플래그
        bool isExpanded = false;
        //랜덤으로 네 방향을 순회하기 위해 배열 선언후 방향 넣어서 초기화
        std::vector<EntryDirection> randomDirection = {EntryDirection::Top, EntryDirection::Right, EntryDirection::Bottom, EntryDirection::Left };
        
        //shuffle에 배열 iterator와 이미 만들어놓은 GetRandomEngine 넣어서 배열 섞어줌
        std::shuffle(randomDirection.begin(), randomDirection.end(), Util::GetRandomEngine());
        
        current = roomQueue.front();
        roomQueue.pop();
        
        for (EntryDirection entry : randomDirection)
        {
            //이미 인자로 받은 방 갯수만큼 모두 생성했을 경우 break
            if (roomCount == roomNum)
            {
                break;
            }

            //방 방향에 맞는 offset값 받아서             
            GridCoord offset = GetOffset(entry);
            GridCoord newCoord = current->coord + offset;
            //unordered_map에서 newCoord의 키가 이미 존재하면 방이 이미 그 좌표에 생성되어 있는 것이기 때문에 방을 가져오고, 아니면 그 좌표에 RoomNode를 새로 생성
            RoomNode& newRoom = dungeon[newCoord];

            
            //방 생성했는데 방이 이미 생성되어 있는 좌표에 생성했으면 continue
            if (newRoom.occupied)
            {
                continue;
            }
            else
            {
                //새로 생긴 노드는 위치가 (0, 0)으로 초기화되기 때문에 직접 넣어줘야 함.
                //위에서 만약 이미 키가 존재했을 때 newRoom에 이미 존재하던 방이 참조가 되더라도 이 연산에 의해 기존 좌표에서 새 좌표로 바뀌진 않으나
                //직관적인 코드를 지향하고 있어서 이렇게 리팩터링
                newRoom.coord = newCoord;
            }

            //50%의 확률로 continue
            //만약 빈 좌표에 새로 방을 생성했는데 여기에 걸리면 occupied가 아직 false이기 때문에 나중에 같은 좌표에 다시 접근해서 또 이 과정을 시도한다. 여러번 방 생성을 시도할 수 있음
            if (Util::RandomRange(0, 1))
            {
                continue;
            }

            //여기까지 통과했으면 방 생성
            newRoom.occupied = true;
            newRoom.distance = current->distance + 1;
            //방 연결
            Connect(current, &newRoom, entry);
            roomQueue.push(&newRoom);
            roomCount++;
            isExpanded = true;

        }
        
        if (!isExpanded)
        {
            endRoomList.push_back(current);
        }
    }

    //보스방이 시작 방과 바로 붙어있는지 확인
    if (roomCount != roomNum)
    {
        return false;
    }

    //끝 방 리스트의 마지막 항복을 bossRoom으로 지정
    if (endRoomList.empty())
    {
        return false;
    }
    bossRoom = endRoomList.back();
    return true;
}

void DungeonMap::Clear()
{



}

bool DungeonMap::Connect(RoomNode* room1, RoomNode* room2, EntryDirection entry)
{   
    //entry를 받아서 room1 -> room2의 방향으로 방을 연결
    switch (entry)
    {
    case EntryDirection::None:
        return false;
    case EntryDirection::Top:
        room1->topRoom = room2;
        room2->bottomRoom = room1;
        break;
    case EntryDirection::Bottom:
        room1->bottomRoom = room2;
        room2->topRoom = room1;
        break;
    case EntryDirection::Left:
        room1->leftRoom = room2;
        room2->rightRoom = room1;
        break;
    case EntryDirection::Right:
        room1->rightRoom = room2;
        room2->leftRoom = room1;
        break;
    default:
        return false;
    }
    return true;
}
