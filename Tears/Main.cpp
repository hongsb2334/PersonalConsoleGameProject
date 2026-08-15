#include <Engine/Engine.h>
//#include <Level/MainLevel.h>
#include <Core/RunState.h>
#include <Util/Util.h>
#include <Level/TitleLevel.h>


int main()
{
    //랜덤 시드 엔진 초기화
    Util::SetRandomSeed();


    //엔진 객체 생성 및 실행
    Craft::Engine engine;
    engine.CreateGameInstance<RunState>();
    engine.AddNewLevel<TitleLevel>();
    engine.Run();
}