#include "Boss.h"
using namespace Craft;

// Boss는 Enemy를 상속받아서 체력을 15, 데미지를 3(Enemy의 15배, 3배씩 잡음)
Boss::Boss(const Craft::Vector2& position) : Enemy(position, 15, 3)
{
    //보스는 "BOSS"로 이미지 변경
    ChangeImage("BOSS");
}
