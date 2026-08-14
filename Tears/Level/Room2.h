#pragma once
#include <Level/Room.h>
class Room2 : public Room
{
    TYPE_DECLARATIONS(Room2, Room)

protected:
    virtual void SpawnDoor() override;
    virtual void SpawnEnemies() override;

};

