#include "Wall.h"

using namespace Craft;
Wall::Wall(const Vector2& position) : Actor("#", position, Color::White)
{
	//벽을 # 글자로 position을 받아 흰색으로 배치할 것임
	// 그리기 우선순위 지정
	sortingOrder = 0;
}

