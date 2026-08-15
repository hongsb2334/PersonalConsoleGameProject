#pragma once
#include <string>
#include <vector>
struct Item
{
    using OnSelected = void(*)();

    //생성자
    Item(const std::string& text, OnSelected onSelected) : text(text), onSelected(onSelected) {}

    //메뉴 텍스트
    std::string text;

    //메뉴를 선택했을 때 실행할 로직
    OnSelected onSelected = nullptr;
};

class SelectMenu
{
public:
    SelectMenu(std::vector<Item> items);

    void Tick();

    inline int GetItemCount() const { return static_cast<int>(items.size()); }
    inline const std::string& GetLabel(int index) const { return items[index].text; }
    inline bool IsSelected(int index) const { return index == selectedIndex; }

private:
    std::vector<Item> items;
    int selectedIndex = 0;
};

