#include "ResultLevel.h"
#include <memory>
#include <Engine/Engine.h>
#include <Render/Renderer.h>
#include <Core/RunState.h>
#include <Level/StartRoom.h>

using namespace Craft;

ResultLevel::ResultLevel(const std::vector<std::string>& title, Craft::Color titleColor, const std::string& filename) : title(title), titleColor(titleColor)
{
    Engine::Get().StopBackGroundMusic();
    Engine::Get().PlayOneShot(filename);

    menu = std::make_shared<SelectMenu>(std::vector<Item>{
        Item("Retry", []()
            {
                Engine::Get().CreateGameInstance<RunState>();
                std::shared_ptr<RunState> runState = Engine::Get().GetGameInstance<RunState>();
                runState->dungeonMap.GenerateRoom(12, 20);
                runState->currentRoom = runState->dungeonMap.GetStartRoom();
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

    int titleX = (Engine::Get().GetWidth() / 2) - (static_cast<int>(title[0].length()) / 2);
    int titleY = (Engine::Get().GetHeight() / 2) - 8;

    for (int i = 0; i < title.size(); i++)
    {
        for (int j = 0; j < title[0].size(); j++)
        {
            Renderer::Get().Submit(title[i], Vector2(titleX, titleY + i), titleColor);
        }
    }
    

    for (int i = 0; i < menu->GetItemCount(); ++i)
    {
        const std::string& label = menu->GetLabel(i);
        int textX = (Engine::Get().GetWidth() / 2) - (static_cast<int>(label.length()) / 2);

        //선택된 항목은 초록색, 아니면 흰색으로 설정
        Color textColor = menu->IsSelected(i) ? Color::Green : Color::White;
        Renderer::Get().Submit(label, Vector2(textX, titleY + static_cast<int>(title.size() )+ 5 + i), textColor);

    }
}
