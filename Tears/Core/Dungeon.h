#pragma once
#include <unordered_map>
//던전 평면도에서의 좌표
struct GridCoord
{
    GridCoord() = default;
    GridCoord(int x, int y) : x(x), y(y) {}

    bool operator==(const GridCoord& other) const
    {
        return ((x == other.x) && (y == other.y));
    }

    GridCoord operator+(const GridCoord& other)
    {
        return GridCoord(x + other.x, y + other.y);
    }

    int x = 0;
    int y = 0;
};

// 해시 테이블, 던전 평면도 좌표를 전달받아서 해시맵의 키 형태로 리턴
struct GridHash 
{
    size_t operator()(const GridCoord& grid)
    {
        //해시값 리턴(정수형)
        return std::hash<int>()(grid.x) * 31 ^ std::hash<int>()(grid.y);
    }
};

struct RoomNode
{
    GridCoord coord;
    RoomNode* topRoom;
    RoomNode* leftRoom;
    RoomNode* rightRoom;
    RoomNode* bottomRoom;

};