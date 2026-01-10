#include "Engine/source/Game.h"
#include <iostream>

int main()
{
    GAMEDEV_ENGINE::Game* gameApp = new GAMEDEV_ENGINE::Game();
    // need to create engine instance
    GAMEDEV_ENGINE::Engine engine;
    // now pass the game to engine
    engine.SetApplication(gameApp);

    // now if initialize of engine is successful then run the engine
    if(engine.Init(1280, 720, "Game DEV Engine"))
    {
        engine.Run();
        std::cout << "Engine run completed, now destroying..." << std::endl;
       
    }
    // after runing to free up resources we destroy the engine
     engine.Destroy();
    
    
    return 0;
}