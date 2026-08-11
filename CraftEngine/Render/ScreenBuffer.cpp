#include "ScreenBuffer.h"
#include <cassert>
#include <iostream>


namespace Craft
{
	ScreenBuffer::ScreenBuffer(const Vector2& screenSize) : size(screenSize)
	{
		//콘솔 버퍼 생성
		buffer = CreateConsoleScreenBuffer(
			GENERIC_READ | GENERIC_WRITE,			//FillConsoleOutputCharacterA 사용하려면 GENERIC_WRITE 권한 필요해서 여기서 지정해줬음
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			nullptr,
			CONSOLE_TEXTMODE_BUFFER,
			nullptr
		);

		assert(buffer != INVALID_HANDLE_VALUE);
        //콘솔 사이즈 구하는 코드
        //COORD maxSize = GetLargestConsoleWindowSize(buffer);
        //std::cout << "Max console size - X: " << maxSize.X << ", Y: " << maxSize.Y << std::endl;
        //std::cin.get();
      
        //// 1단계: 창을 먼저 아주 작게 줄여서, 버퍼보다 항상 작거나 같게 만든다
        //SMALL_RECT minRect = { 0, 0, 1, 1 };
        //SetConsoleWindowInfo(buffer, TRUE, &minRect);

        //// 2단계: 버퍼 크기를 원하는 크기(80x40)로 설정
        //BOOL result = SetConsoleScreenBufferSize(buffer, size);
        //assert(result == TRUE);

        //// 3단계: 이제 버퍼가 충분히 크므로, 창을 최종 크기로 설정 (성공함)
        //SMALL_RECT rect = {};
        //rect.Top = 0;
        //rect.Left = 0;
        //rect.Right = static_cast<short>(size.x - 1);
        //rect.Bottom = static_cast<short>(size.y - 1);
        //result = SetConsoleWindowInfo(buffer, TRUE, &rect);
        //assert(result == TRUE);

        //// 커서 숨기기 (기존 코드 동일)
        //CONSOLE_CURSOR_INFO info;
        //result = GetConsoleCursorInfo(buffer, &info);
        //assert(result == TRUE);
        //info.bVisible = FALSE;
        //result = SetConsoleCursorInfo(buffer, &info);
        //assert(result == TRUE);



		//화면 창 크기 설정, 이 코드에서 창 크기를 80*40으로 설정한 후 
		SMALL_RECT rect = {};
		rect.Top = 0;
		rect.Left = 0;
		rect.Right = static_cast<short>(size.x - 1);
		rect.Bottom = static_cast<short>(size.y - 1);
		BOOL result = SetConsoleWindowInfo(buffer, TRUE, &rect);

		//결과 확인
		assert(result == TRUE);

		//화면 버퍼 크기 설정
		result = SetConsoleScreenBufferSize(buffer, size);
		assert(result == TRUE);

		//직접 만든 콘솔의 커서 끄기
		CONSOLE_CURSOR_INFO info;
		result = GetConsoleCursorInfo(buffer, &info);
		assert(result == TRUE);

		//커서 안보이게 설정
		info.bVisible = FALSE;
		result = SetConsoleCursorInfo(buffer, &info);
		assert(result == TRUE);

	}
	ScreenBuffer::~ScreenBuffer()
	{
		//콘솔 닫기
		if (buffer)
		{
			CloseHandle(buffer);
		}
	}
	void ScreenBuffer::Clear() const
	{
		//콘솔 전체를 지우는 함수
		//공백 문자를 화면 전체에 한번에 설정

		//화면에 설정된 글자 수
		DWORD writtenCount = 0;
		//콘솔 전체 영역을 공백으로 채워 버퍼를 초기화
		//이전 프레임의 잔상을 제거하기 위한 기본 동작
		//Vector2::Zero는 우리가 미리 오버로딩을 만들어 두어서 COORD와 잘 호환된다
		BOOL result = FillConsoleOutputCharacterA(buffer, ' ', size.x * size.y, Vector2::Zero, &writtenCount);
		assert(result == TRUE);
	}
	void ScreenBuffer::Draw(const CHAR_INFO* const charInfo) const
	{
		// charInfo는 2차원 배열(1차원 배열인데 2차원 배열처럼 사용)
		// 설정할 글자 영역
		SMALL_RECT rect = { 
			0,		//left 
			0,		//top
			static_cast<short>(size.x - 1),	//right		마지막 유효 인덱스를 설정하는 부분인데, size.x = 80이라면 79가 마지막 인덱스이기 때문에 size.x - 1을 해준다.
			static_cast<short>(size.y - 1) }; //bottom	right와 같음
		//콘솔에 CHAR_INFO 타입으로 글자 쓰는 함수
		BOOL result = WriteConsoleOutputA(buffer, charInfo, size, Vector2::Zero, &rect);
		assert(result == TRUE);
	}
}