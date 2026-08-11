#pragma once
#include <Level/Level.h>
class MainLevel : public Craft::Level
{
    //초기화 이벤트 함수 오버라이드
    virtual void OnInitialized() override;
};

