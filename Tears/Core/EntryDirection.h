#pragma once
//Player가 레벨에서 시작 할 위치 
enum class EntryDirection
{
    None,
    Top,
    Bottom,
    Left,
    Right
};

inline EntryDirection GetOppositeDirection(EntryDirection entry)
{
    switch (entry)
    {
    case EntryDirection::Top:
        return EntryDirection::Bottom;
    case EntryDirection::Bottom:
        return EntryDirection::Top;
    case EntryDirection::Left:
        return EntryDirection::Right;
    case EntryDirection::Right:
        return EntryDirection::Left;
    default:
        return EntryDirection::None;
    }
    
}