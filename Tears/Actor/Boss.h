#pragma once
//#include <Actor/Enemy.h>
#include <Actor/ChaserEnemy.h>

// Todo: Enemy 클래스는 기본으로 아무 움직임이 없다. 그래서 임시로 Enemy-> ChaserEnemy를 상속하게 함

class Boss : public ChaserEnemy
{
    TYPE_DECLARATIONS(Boss, ChaserEnemy)
    
public:
    Boss(const Craft::Vector2& position);
};

