#include <iostream>
#include "Game.h"
#include "Engine/Engine.h"
#include "Engine/Application.h"

int main()
{
    GAMEDEV_ENGINE::Game* gameApp = new GAMEDEV_ENGINE::Game();
    
    // Create engine instance
    GAMEDEV_ENGINE::Engine& engine = GAMEDEV_ENGINE::Engine::GetInstance();
    
    // Pass the game to engine
    engine.SetApplication(gameApp);
    
    // Initialize and run the engine
    if(engine.Init(1280, 720, "Game DEV Engine"))
    {
        engine.Run();
        std::cout << "Engine run completed, now destroying..." << std::endl;
    }
    
    // Free up resources
    engine.Destroy();
    delete gameApp;
    
    return 0;
}