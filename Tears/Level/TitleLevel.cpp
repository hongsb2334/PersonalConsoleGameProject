#include "TitleLevel.h"
#include <Engine/Engine.h>
#include <Input/Input.h>
#include <Level/StartRoom.h>
using namespace Craft;
void TitleLevel::OnInitialized()
{
    super::OnInitialized();

    std::string title = "Tears";
    std::string gameStart = "Press Enter to start";

    //화면 너비의 절반에서 글자 길이의 절반만큼 왼쪽으로 이동
    int titleX = (Engine::Get().GetWidth() / 2) - (static_cast<int>(title.length()) / 2);
    int titleY = (Engine::Get().GetHeight() / 2) - 2;

    int gameStartX = (Engine::Get().GetWidth() / 2) - (static_cast<int>(gameStart.length()) / 2);
    //title의 위치에서 아래로 3정도 밑에 위치
    int gameStartY = titleY + 3;

    SpawnActor<Actor>(title, Vector2(titleX, titleY), Color::Green);
    SpawnActor<Actor>(gameStart, Vector2(gameStartX, gameStartY), Color::White);

}

void TitleLevel::Tick(float deltaTime)
{
    super::Tick(deltaTime);
    if (Input::Get().GetKeydown(VK_RETURN))
    {
        Engine::Get().AddNewLevel<StartRoom>();
    }
}
