#include "Target.h"
using namespace Craft;

Target::Target(const Vector2& position) : Actor("T", position, Color::Yellow)
{
	//우선순위 설정
	//바닥보다는 크고 박스 및 플레이어는 작은 값
	sortingOrder = 3;
}
