#pragma once
//#include <Actor/Enemy.h>

#include <Actor/Enemy/ChargerEnemy.h>

// Todo: Enemy 클래스는 기본으로 아무 움직임이 없다. 그래서 임시로 Enemy-> ChaserEnemy를 상속하게 함

class Boss : public ChargerEnemy
{
    TYPE_DECLARATIONS(Boss, ChargerEnemy)
    
public:
    Boss(const Craft::Vector2& position);

    static constexpr int spawnWidth = 4;
};

