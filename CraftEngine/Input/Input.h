#pragma once
#include <Core/Core.h>
namespace Craft
{
	class CRAFT_API Input
	{
		//Engine 클래스 friend 선언
		friend class Engine;


		// 키 입력 상태를 저장하는 구조체
		struct KeyState
		{
			//현재 프레임에 키가 눌렸는지 여부
			bool isKeyDown = false;

			//이전 프레임에 키가 눌렸는지 여부
			bool wasKeyDown = false;
		};

	public:
		Input();
		~Input() = default;

		//키 눌림/해제 여부 확인
		//이전 프레임에 안눌렸다가 현재 프레임에 눌리면 true 반환
		bool GetKeydown(int keyCode) const;

		//이전 프레임에 눌렸다가 현재 프레임에 안눌리면 true 반환
		bool GetKeyup(int keyCode) const;

		//현재 프레임에 입력이 눌려 있으면 반복해서 true 반환
		bool GetKey(int keyCode) const;

		//외부에서 접근이 가능하도록 해주는 함수
		static Input& Get();

	private:
		//현재 프레임에 특정 키 입력이 발생했는지를 처리하는 함수
		void ProcessInput();

		//이전 프레임 저장 함수
		void SavePreviousStates();

	private:
		//가상 키 수 (256가지)
		const int keyCount = 256;

		//키 상태를 관리할 배열
		KeyState Keystates[256] = { };

		//전역 접근이 가능하도록 변수 추가
		static Input* instance;
	};
}


