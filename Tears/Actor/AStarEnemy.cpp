#include "AStarEnemy.h"
#include <Actor/Player.h>
#include <Level/Room.h>
#include <Engine/Engine.h>
#include <cmath>
using namespace Craft;

AStarEnemy::AStarEnemy(const Craft::Vector2& position, int maxHp, int damage) : Enemy(position, maxHp, damage)
{
    ChangeImage("AStar");
}

void AStarEnemy::MovingPattern(float& outDx, float& outDy) const
{
    outDx = currentDx;
    outDy = currentDy;
}

void AStarEnemy::Tick(float deltaTime)
{
    super::Tick(deltaTime);
    std::shared_ptr<Room> room = Cast<Room>(GetOwner());
    if (!room)
    {
        return;
    }

    std::shared_ptr<Player> player = room->FindActor<Player>();
    if (!player)
    {
        return;
    }
    
    int enemyWidth = GetWidth();

    //성공 시 path[0] = AStarEnemy의 위치, path[1] = 다음 위치
    std::vector<Vector2> path = astar.FindPath(GetPosition(), player->GetPosition(), Engine::Get().GetWidth(), Engine::Get().GetHeight(), [room, enemyWidth](int x, int y) {
        for (int i = x; i < x + enemyWidth; i++)
        {
            if (room->IsBlocked(i, y))
            {
                return true;
            }
        }
        return false;
        });

    //path는 지역변수라 Tick 한번 돌면 사라지므로 멤버에 currentPath에 저장해둠
    currentPath = path;

    //path 사이즈가 2여야 path[0]과 path[1]이 모두 보장됨. 처음에 !path.empty()로 했었는데 이러면 path[0]이나 path[1]일때도 통과하기 때문에 안됨
    if (path.size() >=2)
    {
        currentDx = path[1].x - path[0].x;
        currentDy = path[1].y - path[0].y;

        float length = std::sqrt(currentDx * currentDx + currentDy * currentDy);
        if (length > 0.0f)
        {
            currentDx /= length;
            currentDy /= length;
        }
    }
    else
    {
        currentDx = 0.0f;
        currentDy = 0.0f;
    }


}
