#pragma once
#include <vector>
#include <Math/Vector2.h>
#include <functional>

using namespace Craft;

//A* 탐색에 사용되는 노드 클래스
class Node
{
public:
    Node(const Vector2& position, Node* parent = nullptr) : position(position), parent(parent) {}
public:
    //노드의 위치 정보
    Vector2 position;

    //노드의 비용 정보
    float gCost = 0.0f;
    float hCost = 0.0f;
    float fCost = 0.0f;

    //최종 경로를 역추적할 때 사용할 부모 노드
    Node* parent = nullptr;
};


class AStar
{
public:
    //이동 방향과 해당 방향의 이동 비용
    struct Direction
    {
        int x = 0;
        int y = 0;
        float cost = 0.0f;
    };

    //그리드 타일의 종류(열거형)
    //char 타입->문자타입 을 숫자화해서 쓰는거임
    //문자 타입으로 활용, / 1바이트 크기를 할당하고 싶을 때 많이 활용
    enum class TileType : char
    {
        Ground  = 0,
        Wall    = 1
    };

    AStar();
    ~AStar();

    //메시지(객체 지향 프로그래밍 용어) - 공개 메소드(인터페이스)

    //경로 탐색 요청 함수
    std::vector<Vector2> FindPath(const Vector2& startPosition, const Vector2& goalPosition, int gridWidth, int gridHeight, std::function<bool(int, int)> isBlockedFn);

    

private:
    //메소드(클래스 또는 구조체가 가지는 멤버 함수 / 공개되지 않은 함수)

    //이전 탐색에 사용한 정보 및 노드를 정리하는 함수
    void Clear();

    //노드 생성전문 함수
    Node* CreateNode(const Vector2& position, Node* parent = nullptr);

    //부모 노드를 역추적해서 경로를 구성하는 함수
    std::vector<Vector2> ConstructPath(Node* destination);

    //휴리스틱(추정 비용) 계산 함수
    float CalculateHeuristic(const Vector2& current, const Vector2& goal) const;


    //이동하려는 좌표가 유효한지 확인
    bool IsInRange(int x, int y) const;

    // 옵션: 대각선 이동이 장애물 모서리를 통과하는지 확인
    bool IsDiagonalBlocked(const Vector2& current, const Direction& direction) const;
    
    //Open 목록 및 Closed 목록 검사 함수
    Node* FindOpenNode(int x, int y) const;

    bool IsInClosedList(int x, int y) const;

    //목적지 인지 확인하는 함수
    bool IsDestination(const Node* node) const;

    
    

private:
    //동적 할당된 모든 노드를 소유하는 목록
    //메모리 관리를 위해 사용
    std::vector<Node*> allocateNodes;

    //탐색할 노드 목록/탐색을 마친 노드 목록
    std::vector<Node*> openList;
    std::vector<Node*> closedList;

    //시작노드/목표 노드
    Node* startNode = nullptr;
    Node* goalNode = nullptr;


    //이번 탐색에서만 유효한 컨텍스트
    std::function<bool(int, int)> isBlockedFn;

    int gridWidth = 0;
    int gridHeight = 0;
    

};
