#include "StartRoom.h"
#include "Actor/Door.h"
#include <Engine/Engine.h>
#include <Level/Room2.h>
#include <Core/RunState.h>
using namespace Craft;

void StartRoom::OnInitialized()
{
    super::OnInitialized();

    std::string moveTutorial = "WSAD : 이동";
    std::string fireGuide = "SPACE : 발사  R : 발사 모드 변경";

    int moveX = (Engine::Get().GetWidth() / 2) - (static_cast<int>(moveTutorial.length()) / 2);
    int fireX = (Engine::Get().GetWidth() / 2) - (static_cast<int>(fireGuide.length()) / 2);

    SpawnActor<Actor>(moveTutorial, Vector2(moveX, Engine::Get().GetHeight() - 4), Color::Green);
    //한칸 아래 배치
    SpawnActor<Actor>(fireGuide, Vector2(fireX, Engine::Get().GetHeight() - 3), Color::Green);
}

void StartRoom::SpawnDoor()
{
    //문을 화면 중앙 상단에 배치할 것임
    //Door 객체가 생성 되기 전에 위치를 계산해야 해서, static const int로 문 길이를 7로 설정해서 사용
    Vector2 doorPosition(Engine::Get().GetWidth() / 2  - (Door::doorLength / 2), 0);

    TrackSpawnedDoor<Door>(doorPosition, []()
        {
            Engine::Get().GetGameInstance<RunState>()->entryDirection = EntryDirection::Bottom;
            Engine::Get().AddNewLevel<Room2>();
        }
    );
}
