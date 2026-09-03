#include "ResultLevel.h"
#include <memory>
#include <Engine/Engine.h>
#include <Render/Renderer.h>
#include <Core/RunState.h>
#include <Level/StartRoom.h>

using namespace Craft;

ResultLevel::ResultLevel(const std::string& title, Craft::Color titleColor, const std::string& filename) : title(title), titleColor(titleColor)
{
    Engine::Get().StopBackGroundMusic();
    Engine::Get().PlayOneShot(filename);

    menu = std::make_shared<SelectMenu>(std::vector<Item>{
        Item("Retry", []()
            {
                Engine::Get().CreateGameInstance<RunState>();
                Engine::Get().GetGameInstance<RunState>()->dungeonMap.GenerateRoom(12, 20);
                Engine::Get().AddNewLevel<StartRoom>();
            }
        ),
            Item("Quit", []()
                {
                    Engine::Get().Quit();
                }
            )
    });
}

void ResultLevel::Tick(float deltaTime)
{
    super::Tick(deltaTime);
    menu->Tick();
}

void ResultLevel::Draw()
{
    super::Draw();

    int titleX = (Engine::Get().GetWidth() / 2) - (static_cast<int>(title.length()) / 2);
    int titleY = (Engine::Get().GetHeight() / 2) - 2;

    Renderer::Get().Submit(title, Vector2(titleX, titleY), titleColor);

    for (int i = 0; i < menu->GetItemCount(); ++i)
    {
        const std::string& label = menu->GetLabel(i);
        int textX = (Engine::Get().GetWidth() / 2) - (static_cast<int>(label.length()) / 2);

        //선택된 항목은 초록색, 아니면 흰색으로 설정
        Color textColor = menu->IsSelected(i) ? Color::Green : Color::White;
        Renderer::Get().Submit(label, Vector2(textX, titleY + 3 + i), textColor);

    }
}
