#include <Engine/Engine.h>
#include <Level/MainLevel.h>
int main()
{
    //엔진 객체 생성 및 실행
    Craft::Engine engine;
    engine.AddNewLevel<MainLevel>();
    engine.Run();
}