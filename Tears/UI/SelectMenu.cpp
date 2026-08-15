#include "SelectMenu.h"
#include <Input/Input.h>
#include <Windows.h>
using namespace Craft;

SelectMenu::SelectMenu(std::vector<Item> items) : items(std::move(items))
{

}

void SelectMenu::Tick()
{
    if (items.empty())
    {
        return;
    }

    int count = GetItemCount();
    if (Input::Get().GetKeydown(VK_UP))
    {
        selectedIndex = (selectedIndex - 1 + count) % count;
    }
    if (Input::Get().GetKeydown(VK_DOWN))
    {
        selectedIndex = (selectedIndex + 1) % count;
    }
    if (Input::Get().GetKeydown(VK_RETURN) && items[selectedIndex].onSelected)
    {
        items[selectedIndex].onSelected();
    }

}
