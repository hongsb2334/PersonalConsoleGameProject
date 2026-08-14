#pragma once
#include <Level/Room.h>
class StartRoom : public Room
{
    TYPE_DECLARATIONS(StartRoom, Room)

protected:
    virtual void SpawnDoor() override;
private:

};

