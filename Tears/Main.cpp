#include <Engine/Engine.h>
#include <Core/RunState.h>
#include <Util/Util.h>
#include <Level/TitleLevel.h>
#include <Windows.h>

int main()
{
    //한글 깨짐 문제 해결
    SetConsoleOutputCP(949);

    //랜덤 시드 엔진 초기화
    Util::SetRandomSeed();


    //엔진 객체 생성 및 실행
    Craft::Engine engine;
    engine.CreateGameInstance<RunState>();
    engine.AddNewLevel<TitleLevel>();
    //둘 중 하나로 호출, 방 생성 함수
    //Todo: 방 생성 함수 수정 필요(기본 12, 20이 적당함)
    engine.GetGameInstance<RunState>()->dungeonMap.GenerateRoom(4, 20);
    //Craft::Engine::Get().GetGameInstance<RunState>()->dungeonMap.GenerateRoom(12, 20);
    engine.Run();
}