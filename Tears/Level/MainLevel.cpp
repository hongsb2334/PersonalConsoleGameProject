#include "MainLevel.h"
#include "Actor/Player.h"

void MainLevel::OnInitialized()
{
    Level::OnInitialized();

    SpawnActor<Player>();
}
