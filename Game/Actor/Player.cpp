#include "Player.h"
#include "Player.h"
#include "Player.h"
#include <Input/Input.h>
#include <level/GameLevel.h>
#include <Game/Game.h>
using namespace Craft;
Player::Player(const Vector2& position) : Actor("P", position, Color::Green)
{
	//우선순위 설정
	//액터중에 가장 높은 값
	sortingOrder = 10;
}

void Player::Tick(float deltaTime)
{
	//상위 계층의 tick 호출
	//Actor::Tick(deltaTime);
	super::Tick(deltaTime);		//언리얼의 Tick 문법

	//ESC 종료 처리
	if (Input::Get().GetKeydown(VK_ESCAPE))
	{
		//종료 처리
		//QuitGame();
        //메뉴 토글
        Game& game = dynamic_cast<Game&>(Engine::Get());
        game.ToggleMenu();
		return;
	}

    //이동 처리를 위해 GameLevel 객체 얻어오기
    std::shared_ptr<GameLevel> level = Cast<GameLevel>(GetOwner());

	//이동 처리
	if (Input::Get().GetKeydown(VK_RIGHT))
	{
		//이동하려는 위치 값 만들기
		Vector2 newPosition = GetPosition();
		newPosition.x += 1;

        //이동 가능 여부 확인
        if (level && level->CanMove(GetPosition(), newPosition))
        {
            //새로운 위치 설정
            SetPosition(newPosition);
        }
	}
	//이동 처리
	if (Input::Get().GetKeydown(VK_DOWN))
	{
		//이동하려는 위치 값 만들기
		Vector2 newPosition = GetPosition();
		newPosition.y += 1;

        //이동 가능 여부 확인
        if (level && level->CanMove(GetPosition(), newPosition))
        {
            //새로운 위치 설정
            SetPosition(newPosition);
        }
	}

	//이동 처리
	if (Input::Get().GetKeydown(VK_LEFT))
	{
		//이동하려는 위치 값 만들기
		Vector2 newPosition = GetPosition();
		newPosition.x -= 1;

        //이동 가능 여부 확인
        if (level && level->CanMove(GetPosition(), newPosition))
        {
            //새로운 위치 설정
            SetPosition(newPosition);
        }
	}
	//이동 처리
	if (Input::Get().GetKeydown(VK_UP))
	{
		//이동하려는 위치 값 만들기
		Vector2 newPosition = GetPosition();
		newPosition.y -= 1;

        //이동 가능 여부 확인
        if (level && level->CanMove(GetPosition(), newPosition))
        {
            //새로운 위치 설정
            SetPosition(newPosition);
        }
	}
}

