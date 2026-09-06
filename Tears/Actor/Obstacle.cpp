#include "Obstacle.h"
#include <Render/Renderer.h>
using namespace Craft;
//빈 문자열 주고 어떤 형태의 obstacle 설치할지는 Draw에서 설정
Obstacle::Obstacle(const Vector2& position, int tileW, int tileH) : Actor("", position, Color::White)
{
    width = tileW;
    height = tileH;
}

void Obstacle::Draw()
{
    if (!IsActive())
        return;
    for (int row = 0; row < height; ++row)
    {
        Renderer::Get().Submit(std::string(width, '#'), Vector2(position.x, position.y + row), color, sortingOrder);
    }
}
