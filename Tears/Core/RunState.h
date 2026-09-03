#pragma once
#include <Core/GameInstance.h>
#include "Dungeon.h"


class RunState : public Craft::GameInstance
{
public:
    //아직 저장된 값이 없다는 의미
    int playerHp = -1;

    //기본값 없음으로 지정
    EntryDirection entryDirection = EntryDirection::None;

    //던전 맵
    DungeonMap dungeonMap;

    //현재 맵에서 플레이어가 어떤 룸에 있는 지 파악하기 위해 멤버 선언
    RoomNode* currentRoom = nullptr;
};