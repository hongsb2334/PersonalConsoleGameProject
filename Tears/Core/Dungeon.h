#pragma once

//던전 평면도에서의 좌표
class GridCoord
{
public:
    GridCoord() = default;
    GridCoord(int x, int y) : x(x), y(y) {}

    bool operator==(const GridCoord& other)
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

// 해시 테이블
class GridCoordHash 
{
public:

    size_t operator()(const GridCoord& grid)
    {
        
    }
};