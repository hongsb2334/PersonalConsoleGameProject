#include "Engine.h"
#include <Level/Level.h>
#include <Input/Input.h>
#include <Render/Renderer.h>
#include "Physics/CollisionSystem.h"
#include <iostream>
//#include <chrono> 얘도 시간관련
#include <Windows.h>	// 언리얼은 chrono 안쓰고 Windows.h씀
#include <cassert>	
#include <SoundSystem/Sound.h>

namespace Craft
{
	Engine* Engine::instance = nullptr;

	Engine::Engine()
	{
		//instance 초기화
		assert(!instance && "instance is not null");
		instance = this;

		//엔진 설정 로드
		LoadEngineSetting();

		//입력 객체 생성
		input = std::make_unique<Input>();

		//렌더러 객체 생성
		renderer = std::make_unique<Renderer>(Vector2(setting.width, setting.height));

        //콜리전 시스템 객체 생성
        collisionSystem = std::make_unique<CollisionSystem>();

        //사운드 시스템 객체 생성
        sound = std::make_unique<Sound>();
	}
	Engine::~Engine()
	{
		instance = nullptr;
	}
	void Engine::Run()
	{
		// 고해상도 타이머 사용.

		// 밀리세컨드 - 1/1000초 ->해상도 1000.
		//	LARGE_INTEGER -> 1/10000000초
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);

		//현재 시간 읽기
		LARGE_INTEGER counter;
		QueryPerformanceCounter(&counter);

		//프레임 계산을 위한 변수
		int64_t current = counter.QuadPart;
		int64_t previous = current;		//? 첫프레임은 이전이라는 게 없음


		//고정 프레임으로 만들기 위한 값
		float oneFrameTime = 1.0f / setting.framerate;

		//엔진 루프
		while (true)
		{
			//종료 조건 처리
			if (isQuit)
			{
				break;
			}

			//프레임 처리

			//입력 처리
			ProcessInput();

			//프레임 시간 계산
			//1. 현재 시간 읽기
			QueryPerformanceCounter(&counter);
			current = counter.QuadPart;

			//2. (현재 시간 - 이전 시간) / 시간 단위 (해상도).
			// -> 초단위로 변환.
			//    예)밀리세컨드(1/1000초) 200밀리세컨드 (200/1000) -> 0.2초
			// 정수값을 float로 캐스팅해줘서 float값으로 만듬
			float deltaTime = static_cast<float>(current - previous) / static_cast<float>(frequency.QuadPart);	

			//고정 프레임 처리
			//프레임 사이에 걸린 시간이 목표 시간보다 더 많이 지났으면 프레임 처리
			if (deltaTime >= oneFrameTime)
			{
				//게임 이벤트 함수 호출
				OnInitialized();

				//게임 이벤트의 초기화 함수(1번만 호출)
				BeginPlay();

				//게임 업데이트
				Tick(deltaTime);

                //충돌 처리
                ProcessCollision();

				//화면 그리기
				Draw();

				//이 위 까지 완료되면 프레임 처리 완료됨

				//레벨 전환 처리
				if (nextLevel)
				{

					//기존 레벨 정리
					if (mainLevel)
					{
						mainLevel.reset();
					}

					//추가 요청된 레벨을 메인 레벨로 설정
					mainLevel = nextLevel;

					//포인터 정리.
					nextLevel.reset();
				}

				//추가 또는 제거 요청된 액터 정리
				if (mainLevel)
				{
					mainLevel->ProcessAddAndDestroyActors();
                    
                    //액터의 이전 상태 업데이트
                    mainLevel->SavePreviousActorStates();
				}
				//입력 상태 저장
				SavePreviousInputStates();

				//현재 시간을 이전 시간에 저장
				previous = current;
			}
		}

		//종료 처리 함수 호출
		Shutdown();
	}
	void Engine::Quit()
	{
		//엔진 종료 플래그 설정
		isQuit = true;
	}
    void Engine::PlayOneShot(const std::string& filename)
    {
        if (!sound)
        {
            return;
        }

        //사운드 시스템 함수 호출, Asset/Sound 경로로 지정했었음
        sound->PlayOneShot(std::string("../Assets/Sound/") + filename);
    }
    void Engine::PlayBackGroundMusic(const std::string& filename)
    {
        if (!sound)
        {
            return;
        }

        //사운드 시스템 함수 호출, Asset/Sound 경로로 지정했었음
        sound->PlayBackgroundMusic(std::string("../Assets/Sound/") + filename);
    }
    void Engine::StopBackGroundMusic()
    {
        if (!sound)
        {
            return;
        }

        sound->StopBackgroundMusic();
    }
	Engine& Engine::Get()
	{
		//디버그 모드에서만 동작
		assert(instance && "instance is null");		// assert는 무조건(필수로) 통과해야하는 조건이 있을 때 사용
		//instance가 있을 때만 밑의 코드 진행, instance가 null이면 밑에서 nullptr을 역참조가 일어나 정의되지 않은 동작 발생.
		return *instance;
		// TODO: 여기에 return 문을 삽입합니다.
	}
	void Engine::ProcessInput()
	{
		assert(input);
		if (!input)
		{
			return;
		}
		input->ProcessInput();
	}
	void Engine::OnInitialized()
	{
		//레벨 초기화 처리
		//예외 처리, 메인 레벨이 nullptr이 아니거나 이미 초기화 된 상태라면 이 함수를 호출할 필요가 없으므로 리턴
		if (!mainLevel || mainLevel->HasInitialized())
		{
			return;
		}

		//초기화 이벤트 호출
		mainLevel->OnInitialized();
	}
	void Engine::BeginPlay()
	{
		if (!mainLevel)
		{
			return;
		}

		//레벨에 이벤트 전달
		mainLevel->BeginPlay();
	}
	void Engine::Tick(float deltaTime)
	{
		if (!mainLevel)
		{
			return;
		}

		mainLevel->Tick(deltaTime);
	}

	void Engine::Draw()
	{
		if (!mainLevel)
		{
			return;
		}
		mainLevel->Draw();				//레벨이 먼저 그려지고 렌더러가 그린다.

		//렌더러에 Draw 이벤트 호출
		if (!renderer)
		{
			return;
		}

		renderer->Draw();
	}

	void Engine::ProcessCollision()
	{
        //예외 처리
        if (!mainLevel)
        {
            return;
        }
        //레벨의 액터 목록을 충돌 시스템에 전달해 처리 진행
        //의존성 주입 (Dependency Injection)
        collisionSystem->ProcessCollision(mainLevel->actorList);    //collisionSystem->ProcessCollision을 하려면 actorList가 꼭 있어야 하는데 collisionSystem은 actorList의 관리를 하지 않음. 
	}

	void Engine::SavePreviousInputStates()
	{
		assert(input && "input should not be null");
		if (!input)
		{
			return;
		}
		input->SavePreviousStates();
	}
	void Engine::Shutdown()
	{
	}
	void Engine::LoadEngineSetting()
	{
		//파일 열기(개행 문자 처리를 쉽게 텍스트 모드로 열기)
		FILE* file = nullptr;
		fopen_s(&file, "../Config/Setting.txt", "rt");
		if (!file)
		{
			std::cout << "Failed to open engine setting file.\n";

			//디버그 모드에서 강제 중단시키는 기능
			__debugbreak();
			return;
		}

		//데이터 읽어오기
		const int buffersize = 2048;
		char buffer[buffersize] = {};

		size_t readSize = fread(buffer, sizeof(char), buffersize, file); //반환값을 쓰지는 않는데 안받으면 오류떠서 받음

		//값 저장을 위해 서식 해석(파싱-parsing)
		//문자열 자르기(split)
		char* context = nullptr;
		char* token = nullptr;
		//파일에서 읽은 전체 문자열을 개행(\n)기준으로 처음 자르기
		token = strtok_s(buffer, "\n", &context);

		//반복해서 자르기
		while (token)
		{
			//공백 전까지 읽은 문자열을 저장할 변수
			char key[15] = {};

			//포맷을 지정한 문자열 읽기
			//공백 문자를 만나면 그 전까지 읽어서 저장됨(sscanf_s 기능)
			sscanf_s(token, "%s", key, 15);

			//키 값을 비교해서 값 설정
			if (strcmp(key, "framerate") == 0)
			{
				sscanf_s(token, "framerate = %f", &setting.framerate);
			}
			else if (strcmp(key, "width") == 0)
			{
				sscanf_s(token, "width = %d", &setting.width);
			}
			else if (strcmp(key, "height") == 0)
			{
				sscanf_s(token, "height = %d", &setting.height);
			}

			//나머지 문자열 자르기(개행 문자 기준으로)
			token = strtok_s(nullptr, "\n", &context);
		}

		//파일 닫기
		fclose(file);
		file = nullptr;

	}
}