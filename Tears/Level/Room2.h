#pragma once
#include <Level/Room.h>
class Room2 : public Room
{
    TYPE_DECLARATIONS(Room2, Room)

private:
    virtual void OnInitialized() override;
    virtual void Tick(float deltaTime) override;
public:

};

