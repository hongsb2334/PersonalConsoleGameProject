#define NOMINMAX

#include "AStar.h"
#include <algorithm>

AStar::AStar()
{

}

AStar::~AStar()
{
    //초기화
    Clear();
}

//Todo: 최적화 하기, vector를 그대로 리턴함, 
//최적화 -> 안해도 되는 계산을 찾아서 안하게 만드는 것
std::vector<Vector2> AStar::FindPath(const Vector2& startPosition, const Vector2& goalPosition, int gridWidth, int gridHeight, std::function<bool(int, int)> isBlockedFn)
{
    //이전에 탐색한 결과 초기화
    Clear();

    this->gridWidth = gridWidth;
    this->gridHeight = gridHeight;
    this->isBlockedFn = isBlockedFn;

    //예외처리
    if (gridWidth <= 0 || gridHeight <=0)
    {
        //유효하지 않으면 빈 배열 반환
        //return std::vector<Vector2>();
        return {};
    }

    // 시작위치/목표위치가 grid 기준에서 문제 없는 위치값인지 확인
    if (!IsInRange(startPosition.x, startPosition.y) || !IsInRange(goalPosition.x, goalPosition.y))
    {
        return {};
    }

    //시작 위치/목표위치가 이동 불가하면 종료
    if (isBlockedFn(startPosition.x, startPosition.y) || isBlockedFn(goalPosition.x, goalPosition.y))
    {
        return {};
    }


    //탐색 시작
    //시작/목표 노드 생성
    startNode = CreateNode(startPosition);
    goalNode = CreateNode(goalPosition);

    //시작 노드의 비용 계산 및 openList에 추가해 탐색 시작
    startNode->gCost = 0.0f;
    startNode->hCost = CalculateHeuristic(startPosition, goalPosition);
    startNode->fCost = startNode->gCost + startNode->hCost;

    openList.emplace_back(startNode);

    //편의를 위해 사전 비용 설정
    //Todo: 중복 코드 #define 생각
    const float diagonalCost = 1.41421f;
    const std::vector<Direction> directions
    {
        {0, -1, 1.0f}, {0, 1, 1.0f}, //상하
        {-1, 0, 1.0f}, {1, 0, 1.0f}, //좌우
        {-1, -1, diagonalCost}, //좌상단
        {1, -1, diagonalCost}, //우상단
        {-1, 1, diagonalCost}, //좌하단
        {1, 1, diagonalCost}, //우하단
    };

    //openList가 빌 때 까지탐색 반복
    while (!openList.empty())
    {
        
        //openList에서 fCost가 가장 작은 노드 선택
        //Todo: 최적화 하기 (이진 힙 사용해서, 여기는 순차탐색함)
        Node* currentNode = openList[0];
        for (Node* node : openList)
        {
            //더 작은 비용의 노드 검색
            if (node->fCost < currentNode->fCost || (node->fCost == currentNode->fCost && node->hCost < currentNode->hCost))
            {
                currentNode = node;
            }
        }
        //목표 경로인지 확인
        if (IsDestination(currentNode))
        {
            //이동 경로 제작 후 반환
            return ConstructPath(currentNode);
        }

        //목표가 아니면
        //현재 노드를 openList에서 제거
        //방문 처리를 위해
        auto iterator = std::find(openList.begin(), openList.end(), currentNode);
        //검색에 성공했는지 확인
        if (iterator != openList.end())
        {
            // openList 목록에서 제거
            openList.erase(iterator);
        }

        //탐색을 마친 노드를 closedList에 추가
        closedList.emplace_back(currentNode);

        //현재 위치를 기준으로 주변(8방향)의 이웃노드 탐색
        for (const Direction& direction : directions)
        {
            //현재 노드를 기준으로 인접한 노드의 좌표 계산
            //새로운 좌표(위치) = 현재 위치 + 이동 방향 (위치끼리 더하면 잘못된걸 기억하자)
            int newX = currentNode->position.x + direction.x;
            int newY = currentNode->position.y + direction.y;

            //예외처리
            if (!IsInRange(newX, newY))
            {
                continue;
            }

            //새로운 위치가 장애물인지 확인
            if (isBlockedFn(newX, newY))
            {
                continue;
            }

            //대각선 이동 시 장애물을 통과하는 지 확인
            if (IsDiagonalBlocked(currentNode->position, direction))
            {
                continue;
            }

            //이미 방문한 곳이면 건너뛰기
            if (IsInClosedList(newX, newY))
            {
                continue;
            }

            //현재 노드를 거쳐서 새로운 위치로 가는데 드는 비용 계산
            float newGCost = currentNode->gCost + direction.cost;

            //이미 openList에 있는데 비용면에서 더 나은지 확인
            Node* openNode = FindOpenNode(newX, newY);
            if (openNode)
            {
                //비용 비교
                if (newGCost < openNode->gCost)
                {
                    openNode->gCost = newGCost;
                    openNode->fCost = newGCost + openNode->hCost;
                    openNode->parent = currentNode;
                }

                continue;
            }

            //이웃 노드 생성 및 openList에 추가
            Node* neighborNode = CreateNode(Vector2(newX, newY), currentNode);

            //새로운 노드의 비용 계산
            neighborNode->gCost = newGCost;
            neighborNode->hCost = CalculateHeuristic(neighborNode->position, goalNode->position);
            neighborNode->fCost = neighborNode->gCost + neighborNode->hCost;

            //새로운 노드를 openList에 추가
            openList.emplace_back(neighborNode);

        }
    }

    //빈 경로 반환(탐색 실패)
    //return std::vector<Vector2>();
    return {};
}



void AStar::Clear()
{
    //탐색 과정에서 생성했던 모든 노드 삭제 및 해제
    for (Node*& node : allocateNodes)
    {
        delete node;
        node = nullptr;
    }

    allocateNodes.clear();
    openList.clear();
    closedList.clear();

    startNode = nullptr;
    goalNode = nullptr;
}

Node* AStar::CreateNode(const Vector2& position, Node* parent)
{
    //노드를 생성하고, allocatedNodes에 추가
    Node* newNode = new Node(position, parent);
    allocateNodes.emplace_back(newNode);
    
    //생성한 노드 반환
    return newNode;
}

std::vector<Vector2> AStar::ConstructPath(Node* destination)
{
    //목표 노드로부터 부모 노드를 따라 경로 역추적
    std::vector<Vector2> path;
    Node* current = destination;

    while (current)
    {
        //현재 노드는 경로 배열에 추가
        path.emplace_back(current->position);

        //부모 노드로 이동해서 경로 역추적
        current = current->parent;
    }

    //루프가 종료되면 path에는 반대 방향의 경로 정보가 저장됨.
    //따라서 다시 역방향으로 뒤집기 필요
    std::reverse(path.begin(), path.end());

    return path;
}

float AStar::CalculateHeuristic(const Vector2& current, const Vector2& goal) const
{
    // 옥타일 거리(Octaile Distance) 계산 방식을 사용.
    // 8방향으로 이동을 허용할 때 사용 가능한 계산 방식 중 하나.
    // 1.현재 위치에서 목표 위치까지의 차이를 가로/세로 이동 간격으로 계산한 뒤
    // 2.대각선으로 이동이 가능한 거리는 최대한 대각선 이동 거리로 좁히고
    // 3.직선으로만 이동 가능한 거리만 계산
    // 4.hCost는 아래의 계산 공식으로 계산.
    //   (대각선 이동 횟수 x 대각선 이동 비용(1.414)) + (직선 이동 횟수 x 직선 이동 비용 (1))

    //대각선 이동 허용시 주의사항
    //대각선 형태의 장애물을 뚫고가지 못하게 막아야 함. (장애물이 대각선형태로 있을 때 위, 오른쪽 에 장애물이 있으면 못가게 막음)

    //현재 위치와 목표 위치 사이의 차이 계산
    //std::abs ->절대값 함수
    int diffX = std::abs(current.x - goal.x);
    int diffY = std::abs(current.y - goal.y);

    //대각선 거리와 남은 직선 거리 분리
    int diagonalDistance = std::min(diffX, diffY);
    int straightDistance = std::max(diffX, diffY) - diagonalDistance;

    //대각선 비용
    const float diagonalCost = 1.41421f;
    const float straightCost = 1.0f;



    return diagonalDistance * diagonalCost + straightDistance * straightCost;
}



bool AStar::IsInRange(int x, int y) const
{
    //grid의 가로 크기는 같다고 가정

    return ((x >= 0 && x < gridWidth) && (y >=0 && y < gridHeight));

}

bool AStar::IsDiagonalBlocked(const Vector2& current, const Direction& direction) const
{
    //이동하려는 방향(대각선) 방향에 장애물이 있는지 확인
    //대각선 성분만 판단
    //대각선 성분이 아니라면 판단할 필요 없음 (대각선 방향은 좌표상 0이 없음. (1, 1), (1, -1) 이 꼴임)
    if (direction.x == 0 || direction.y == 0)
    {
        return false;
    }

    //대각선으로 이동하려는 새로운 위치의 x성분과 y성분을 분해
    int sideX = current.x + direction.x;
    int sideY = current.y + direction.y;

    //대각선 이동 성분 위치 중 하나라도 장애물(벽) 있으면 이동 불가 (여기선 위, 오른쪽 중 둘중 하나만 장애물 있어도 대각선 통과 못하게 했음)
    return isBlockedFn(sideX, current.y) || isBlockedFn(current.x, sideY);

}

Node* AStar::FindOpenNode(int x, int y) const
{
    //같은 좌표의 노드를 OpenList에서 찾기
    //Todo: 여기선 순차탐색 쓰고 있는데 이진 탐색 같은걸로 최적화 여지 있음
    for (Node* node : openList)
    {
        //좌표 비교
        if (node->position == Vector2(x, y))
        {
            return node;
        }
    }
    return nullptr;
}

bool AStar::IsInClosedList(int x, int y) const
{
    //같은 좌표가 ClosedList에 있는지 확인
    for (Node* node : closedList)
    {
        //좌표 비교
        if (node->position == Vector2(x, y))
        {
            return true;
        }
    }

    return false;
}

bool AStar::IsDestination(const Node* node) const
{
    //두 노드 모두 nullptr이 아니고, 두 노드의 위치가 같은 지 비교
    return node && goalNode && node->position == goalNode->position;
}
