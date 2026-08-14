#pragma once
#include <Level/Room.h>

class BossRoom : public Room
{
    virtual void SpawnDoor() override;
    virtual void SpawnEnemies() override;

};

