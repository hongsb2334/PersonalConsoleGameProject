<div align="center">

# 🎯 Tears

### 콘솔(cmd) 환경에서 동작하는 로그라이크 슈팅 게임

Room / Door 던전 구조 · Damageable 전투 시스템 · GameInstance 상태 유지 · XAudio2 사운드를 갖춘, *Binding of Isaac*에서 영감을 받은 콘솔 슈팅 게임

[![C++](https://img.shields.io/badge/C%2B%2B-17-00599C?style=for-the-badge&logo=cplusplus)](https://isocpp.org/)
[![Engine](https://img.shields.io/badge/Engine-CraftEngine-313131?style=for-the-badge)](#)
[![Platform](https://img.shields.io/badge/Platform-Windows%20Console-lightgrey?style=for-the-badge&logo=windows)](#)


</div>

<br>

<div align="center">
  <img src="images/gameplay.gif" alt="Tears Gameplay" width="90%">
  <p><i><img width="400" height="225" alt="image" src="https://github.com/user-attachments/assets/7e369c61-aea0-4210-a09f-e23198084ec0" /></i></p>
</div>

<br>

## 📋 목차

- [소개](#-소개)
- [조작법 & 승패 조건](#-조작법--승패-조건)
- [주요 기능](#-주요-기능)
  - [Room / Door 던전 시스템](#-room--door-던전-시스템)
  - [Damageable 전투 시스템](#-damageable-전투-시스템)
  - [GameInstance / RunState](#-gameinstance--runstate)
  - [사운드 시스템](#-사운드-시스템)
  - [UI & 승패 화면](#-ui--승패-화면)
- [기술 스택](#-기술-스택)
- [시작하기](#-시작하기)
- [프로젝트 구조](#-프로젝트-구조)
- [개발 회고](#-개발-회고)


<br>

## 📖 소개

**Tears**는 수업에서 사용한 자체 프레임워크 **CraftEngine**을 기반으로, 그래픽 없이 Windows 콘솔(cmd) 위에서 동작하도록 만든 로그라이크 슈팅 게임입니다. *Binding of Isaac*의 방 단위 던전 탐색과 탄막 슈팅 재미를 텍스트 렌더링 환경 안에서 구현하는 데 초점을 맞췄으며, 엔진 프레임워크 자체를 복습하는 것을 개발 목표로 삼았습니다.

<br>

## 🎮 조작법 & 승패 조건

| 키 | 동작 |
|---|---|
| 방향키 | 이동 |
| Space | 발사 |
| R | 발사 모드 변경 |

던전을 돌파하며 적들을 물리치고 **Boss**까지 처치하면 승리, 그 전에 체력이 모두 닳으면 패배합니다.

<br>

## ✨ 주요 기능

### 🚪 Room / Door 던전 시스템

- 방(Room) 단위로 구성된 던전을 Door를 통해 이동하며 탐색
- 문의 위치를 기준으로 다음 레벨 진입 시 플레이어 스폰 위치를 자연스럽게 배치

### 💥 Damageable 전투 시스템

- Player와 Enemy가 공통으로 상속받는 **`Damageable`** 클래스로 피격 · 체력 처리를 통일
- 발사체(Player Projectile / Enemy Projectile) 충돌 및 데미지 로직 구현

### 🎮 GameInstance / RunState

- CraftEngine은 레벨 전환 시 현재 레벨을 파괴하고 액터를 재생성하는 구조라, 별도 처리가 없으면 HP 등 플레이어 상태가 초기화됨
- 언리얼의 GameInstance 개념을 참고해 Engine 계층에 껍데기 클래스를, 게임 계층에 이를 상속한 **`RunState`** 클래스를 만들어 HP와 다음 레벨 스폰 위치를 레벨 전환 간 유지

### 🔊 사운드 시스템

- **XAudio2** 기반 사운드 재생 (SoundSystem)

### 🖥 UI & 승패 화면

- SelectedMenu(선택 메뉴) 구현
- 게임 승리 / 패배 화면 구현
- Boss 및 아이템(Item) 시스템

<br>

## 🛠 기술 스택

| 분류 | 사용 기술 |
|---|---|
| 엔진 | CraftEngine (수업용 자체 프레임워크) |
| 언어 | C++ |
| 사운드 | XAudio2 |
| 플랫폼 | Windows Console (cmd) |

<br>

## 🚀 시작하기

### 요구 사항

- Windows
- Visual Studio 2022 (C++ 프로젝트 빌드용)
- CraftEngine (수업에서 제공된 프레임워크 — 여기에 필요한 설정/의존성을 적어주세요)

### 실행 방법

```bash
git clone https://github.com/{your-username}/Tears.git
cd Tears
```

1. `(솔루션 파일명).sln`을 Visual Studio로 연다.
2. 빌드 후 실행한다.
3. Windows 터미널 기준 창 크기가 **120x30**일 때 가장 정상적으로 보인다. (콘솔 호스트에서는 창 크기가 다르게 표시될 수 있음)

<br>

## 📁 프로젝트 구조

```
Tears/
├── Source/
│   ├── Engine/          # CraftEngine 코어, GameInstance 껍데기
│   ├── Game/
│   │   ├── Characters/  # Player, Enemy, Damageable
│   │   ├── Weapons/     # Projectile
│   │   ├── Level/       # Room, Door
│   │   ├── RunState/    # 레벨 전환 간 상태 유지
│   │   ├── UI/          # SelectedMenu, 승/패 화면
│   │   └── Sound/       # SoundSystem (XAudio2)
└── README.md
```

<br>

## 📝 개발 회고

개발 일정, 잘했던 점과 부족했던 점, 마주쳤던 문제(터미널 창 크기 이슈, 레벨 전환 시 HP 초기화 문제)와 해결 과정, 다음에 시도해보고 싶은 개선 사항은 아래 글에 정리했습니다.

- https://velog.io/@hongsb2334/%ED%9A%8C%EA%B3%A0-ConsoleGameProject-Part1

<br>



<br>

<div align="center">

made with 🖥️ CraftEngine

</div>
