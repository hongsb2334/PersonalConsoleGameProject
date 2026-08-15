#pragma once
#include <Actor/Enemy.h>
class Boss : public Enemy
{
    TYPE_DECLARATIONS(Boss, Enemy)
    
public:
    Boss(const Craft::Vector2& position);
};

