#pragma once

class Timer
{
public:
    Timer(float targetTime = 1.0f);

    //타이머 시간 업데이트 함수
    void Tick(float deltaTime);

    //경과시간 리셋 함수       Reset과 SetTargetTime은 구현이 간단해서 인라인으로 만들어도 됨
    void Reset();

    //목표시간 재설정 함수
    void SetTargetTime(float targetTime);

    //설정한 시간이 지났는지 확인
    inline bool IsTimeOut() const { return elapsedTime >= targetTime; }


private:
    //경과 시간 계산용 변수
    float elapsedTime = 0.0f;

    //타이머 목표 시간
    float targetTime = 0.0f;


};

