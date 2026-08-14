#pragma once
#include <Level/Room.h>

class Room4 : public Room
{
    TYPE_DECLARATIONS(Room4, Room)

    virtual void SpawnDoor() override;
    virtual void SpawnEnemies() override;
};

