#pragma once
#include <Level/Room.h>

class BossRoom : public Room
{
    TYPE_DECLARATIONS(BossRoom, Room)

    virtual void SpawnDoor() override;
    virtual void SpawnEnemies() override;
    virtual void OnRoomCleared() override;
    virtual void Draw() override;


};

