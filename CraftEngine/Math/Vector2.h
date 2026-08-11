#pragma once
#include <Core/Core.h>
#include <Windows.h>


namespace Craft
{
	class CRAFT_API Vector2					//좌표가 왼쪽 위 기준으로 (0, 0) 아래로 내려갈수록 y값 증가
	{
	public:
		Vector2(int x = 0, int y = 0);
		~Vector2() = default;

		//연산자 오버로딩
		
		//windows 콘솔 좌표계로 변환하는 연산자 오버로딩
		operator COORD() const;
		operator COORD();

		//일반 연산자 오버로딩
		Vector2 operator+(const Vector2& other) const;	//이항 연산자 (멤버함수로 구현하였으므로 왼쪽 항이 자기자신)
		Vector2 operator-(const Vector2& other) const;
		Vector2 operator*(const Vector2& other) const;
		Vector2 operator/(const Vector2& other) const;

		//대입 연산자 오버로딩
		Vector2& operator=(const Vector2& other);

		//비교 연산자 오버로딩
		bool operator==(const Vector2& other)const;
		bool operator!=(const Vector2& other)const;

		//자주 사용할 값을 전역 변수로 선언
		static Vector2 Zero;
		static Vector2 One;
		static Vector2 Right;
		static Vector2 Up;


	public:
		//좌표계 값으로 사용하기 위한 변수
		int x = 0;
		int y = 0;
	};
}


