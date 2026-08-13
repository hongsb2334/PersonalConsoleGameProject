#include <Engine/Engine.h>
//#include <Level/MainLevel.h>
#include <Level/StartRoom.h>
int main()
{
    //엔진 객체 생성 및 실행
    Craft::Engine engine;
    engine.AddNewLevel<StartRoom>();
    engine.Run();
}