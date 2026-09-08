#include "AStarEnemy.h"
#include <Actor/Player.h>
#include <Level/Room.h>
#include <Engine/Engine.h>
#include <cmath>
using namespace Craft;

AStarEnemy::AStarEnemy(const Craft::Vector2& position, int maxHp, int damage) : Enemy(position, maxHp, damage)
{
    ChangeImage("AStar");
    color = Color::BrightWhite;
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

    ////Todo: 클램프 넣어주긴 했는데 AStar는 플레이어 뒤로 넘어가지 않아서 의미 없음.
    ////x값이 0보다 작아지면 0으로 클램프 후 부호 바꿔서 반대로 진행하게 함
    //if (xPosition < 1.0f)
    //{
    //    xPosition = 1.0f;
    //}
    ////x값과 가로 길이 더한게 창 넘어가면 창에서 글자 길이 빼서 클램프 해주고 방향 반대 설정
    //if (xPosition + width > Engine::Get().GetWidth() - 1)
    //{
    //    xPosition = Engine::Get().GetWidth() - width - 1;
    //}
    ////x값과 마찬가지
    //if (yPosition < 1.0f)
    //{
    //    yPosition = 1.0f;
    //}
    ////Todo: x값이랑 똑같은데 현재 height는 1이다, 나중에 2차원 액터 사용시 수정 필요
    //if (yPosition + height > Engine::Get().GetHeight() - 1)
    //{
    //    yPosition = Engine::Get().GetHeight() - height - 1;
    //}


}
