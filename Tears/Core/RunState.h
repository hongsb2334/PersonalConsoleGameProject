#pragma once
#include <Core/GameInstance.h>

//Player가 레벨에서 시작 할 위치 
enum class EntryDirection
{
    None,
    Top,
    Bottom,
    Left,
    Right
};

class RunState : public Craft::GameInstance
{
public:
    //아직 저장된 값이 없다는 의미
    int playerHp = -1;

    //기본값 없음으로 지정
    EntryDirection entryDirection = EntryDirection::None;
};