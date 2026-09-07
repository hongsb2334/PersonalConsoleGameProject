#pragma once
#include <Actor/Enemy.h>
#include <AStar/AStar.h>
class AStarEnemy : public Enemy
{
    TYPE_DECLARATIONS(AStarEnemy, Enemy)

public:

    //AStar 알고리즘을 사용하는 적, ChaserEnemy와 접근 방식은 비슷하나, AStarEnemy는 장애물을 막힌 칸으로 인식하고 탐색 후보에서 제외한다.
    AStarEnemy(const Craft::Vector2& position, int maxHp = 2, int damage = 3);

    virtual void MovingPattern(float& outDx, float& outDy) const override;

    virtual void Tick(float deltaTime) override;

    std::vector<Vector2> GetCurrentPath() const { return currentPath; }

    static constexpr int spawnWidth = 5;
private:
    //현재 이동중인 방향
    float currentDx = 0.0f;
    float currentDy = 0.0f;

    Timer timer;

    //A* 객체
    AStar astar;
    //경로를 저장할 멤버 선언
    std::vector<Vector2> currentPath;



};

