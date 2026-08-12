#include "MainLevel.h"
#include "Actor/Player.h"
#include "Actor/Enemy.h"
#include "Actor/EnemySpawner.h"

void MainLevel::OnInitialized()
{
    Level::OnInitialized();

    SpawnActor<Player>();
    SpawnActor<EnemySpawner>();
    
}
