#pragma once
#include <Level/Room.h>
class StartRoom : public Room
{
    TYPE_DECLARATIONS(StartRoom, Room)
        
public:
    virtual void OnInitialized() override;

protected:
    virtual void SpawnDoor() override;
private:

};

