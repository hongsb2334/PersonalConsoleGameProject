#pragma once
#include <unordered_map>
#include <vector>
#include "EntryDirection.h"

//던전 평면도에서의 좌표
struct GridCoord
{
    GridCoord() = default;
    GridCoord(int x, int y) : x(x), y(y) {}

    bool operator==(const GridCoord& other) const
    {
        return ((x == other.x) && (y == other.y));
    }

    GridCoord operator+(const GridCoord& other) const
    {   
        return GridCoord(x + other.x, y + other.y);
    }

    int x = 0;
    int y = 0;
};

// 해시 테이블, 던전 평면도 좌표를 전달받아서 해시맵의 키 형태로 리턴
struct GridHash 
{
    size_t operator()(const GridCoord& grid) const
    {
        //해시값 리턴(정수형)
        return std::hash<int>()(grid.x) * 31 ^ std::hash<int>()(grid.y);
    }
};

//던전의 방 하나
struct RoomNode
{
    GridCoord coord;
    RoomNode* topRoom = nullptr;
    RoomNode* leftRoom = nullptr;
    RoomNode* rightRoom = nullptr;
    RoomNode* bottomRoom = nullptr;

    //
    bool occupied = false;
    //방과 방끼리의 거리, -1로 초기화한것은 아직 값이 할당되지 않았음을 의미
    int distance = -1 ;
};

class DungeonMap
{
public:
    //방 생성하는 함수, 최대 방 개수와 최대 재시도 횟수를 인자로 받는다.
    bool GenerateRoom(int roomNum, int maxRegenerateNum);

    //좌표로 노드 찾는 함수
    RoomNode* FindRoom(const GridCoord& coord);
    const std::vector<RoomNode*>& GetEndRoomList() const { return endRoomList; }

    //시작방/보스방 꺼내는 함수
    RoomNode* GetStartRoom() const { return startRoom; }
    RoomNode* GetBossRoom() const { return bossRoom; }

private:
    //방 생성 한번 시도 함수
    bool GenerateRoomOnce(int roomNum);
    
    //이전 데이터 지우는 함수
    void Clear();
    
    //두 방 노드들을 연결하는 함수
    bool Connect(RoomNode* room1, RoomNode* room2, EntryDirection entryDirection);

    //랜덤 방 방향 받아서 그에 맞는 좌표 offset 값 리턴하는 함수
    GridCoord GetOffset(EntryDirection entry);

private:
    //던전
    std::unordered_map<GridCoord, RoomNode, GridHash> dungeon;
    
    //시작방과 보스방 가리키는 포인터
    RoomNode* startRoom = nullptr;
    RoomNode* bossRoom = nullptr;

    //방 개수 세는 변수
    int roomCount = 0;
    
    //더이상 확장 못하는 끝방을 담을 배열
    std::vector<RoomNode*> endRoomList;

};