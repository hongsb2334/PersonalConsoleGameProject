#include "StartRoom.h"
#include "Actor/Door.h"
#include <Engine/Engine.h>
#include <Core/RunState.h>
using namespace Craft;

void StartRoom::OnInitialized()
{
    super::OnInitialized();

    std::string moveTutorial = "방향키 : 이동";
    std::string fireGuide = "SPACE : 발사  R : 발사 모드 변경";

    int moveX = (Engine::Get().GetWidth() / 2) - (static_cast<int>(moveTutorial.length()) / 2);
    int fireX = (Engine::Get().GetWidth() / 2) - (static_cast<int>(fireGuide.length()) / 2);

    SpawnActor<Actor>(moveTutorial, Vector2(moveX, Engine::Get().GetHeight() - 4), Color::Green);
    //한칸 아래 배치
    SpawnActor<Actor>(fireGuide, Vector2(fireX, Engine::Get().GetHeight() - 3), Color::Green);
}


