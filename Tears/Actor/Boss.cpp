#include "Boss.h"
using namespace Craft;

// Boss는 Enemy를 상속받아서 체력을 10, 데미지를 2(Enemy의 10배, 2배씩 잡음)
Boss::Boss(const Craft::Vector2& position) : Enemy(position, 10, 2)
{
    //보스는 "BOSS"로 이미지 변경
    ChangeImage("BOSS");
}
