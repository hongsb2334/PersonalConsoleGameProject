#include "input.h"
#include <cassert>
#include <Windows.h>

namespace Craft
{
	//static 변수 초기화
	Input* Input::instance = nullptr;

	
	Input::Input()
	{
		//시작할 때 instance 값은 null이어야 함
		assert(!instance && "instance should be null");
		instance = this;
	}
	bool Input::GetKeydown(int keyCode) const
	{
		return !Keystates[keyCode].wasKeyDown && Keystates[keyCode].isKeyDown;
	}
	bool Input::GetKeyup(int keyCode) const
	{
		return Keystates[keyCode].wasKeyDown && !Keystates[keyCode].isKeyDown;
	}
	bool Input::GetKey(int keyCode) const
	{
		return Keystates[keyCode].isKeyDown;
	}
	Input& Input::Get()
	{
		assert(instance);
		return *instance;
	}
	void Input::ProcessInput()
	{
		// 현재 프레임에 키 입력이 발생했는지 확인
		for (int i = 0; i < keyCount;++i)
		{
			//키 눌림 여부 저장
			//0x8000은 최상위 비트, 키가 눌려있다면 GetAsyncKeyState 값은 0이 아니므로 AND 연산 값이 0이 아니다. 값이 안눌려 있다면 비트 값이 0이기 때문에 AND연산하면 0이 된다. 따라서 눌려 있는지, 안눌려 있는지를 비트 연산으로 판단
			Keystates[i].isKeyDown = ((GetAsyncKeyState(i) & 0x8000) != 0);	//실패하거나 입력이 들어오지 않으면 0이 됨. 0이 아니라면 입력이 들어온것
		}
	}
	void Input::SavePreviousStates()
	{
		//이전 프레임 입력 값 저장
		for (KeyState& state : Keystates)
		{
			state.wasKeyDown = state.isKeyDown;
		}
	}
}