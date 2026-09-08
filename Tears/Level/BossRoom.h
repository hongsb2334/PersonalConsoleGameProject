#pragma once
#include <Level/Room.h>
#include <Actor/Boss.h>
class BossRoom : public Room
{
    TYPE_DECLARATIONS(BossRoom, Room)

    virtual void SpawnDoor() override;
    virtual void SpawnEnemies() override;
    virtual void OnRoomCleared() override;
    virtual bool IsRoomCleared() const override;
    virtual void Draw() override;
    
    std::shared_ptr<Boss> bossRef;

};

