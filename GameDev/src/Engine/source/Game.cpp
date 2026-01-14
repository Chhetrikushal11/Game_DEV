
#include "Engine/source/Game.h"
#include "Engine/source/Application.h"
#include "Engine/source/graphics/GraphicsAPI.h"

 namespace GAMEDEV_ENGINE 
 {
bool Game::Init()
{
    std::string vertexShader = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;
        void main()
        {
            gl_Position = vec4(aPos, 1.0);
        }
    )";
    
    std::string fragmentShader = R"(
        #version 330 core
        out vec4 FragColor;
        void main()
        {
            FragColor = vec4(1.0, 0.5, 0.2, 1.0);
        }
    )";
    
    auto& graphicsAPI = Engine::GetInstance().GetGraphicsAPI();
    auto shaderProgram = graphicsAPI.CreateShaderProgram(vertexShader, fragmentShader);
    
    if(!shaderProgram)  // If shader creation FAILED (nullptr)
    {
        std::cerr << "Failed to create shader program" << std::endl;  // ✅ Error message
        return false;  // ✅ Return false (failure)
    }
    else  // If shader creation SUCCEEDED
    {
        std::cout << "Game Initialized" << std::endl;  // ✅ Success message
        return true;  // ✅ Return true (success)
    }
}

    void Game::Update(float deltaTime)
    {
       // std::cout <<"Current deltaTime: "<< deltaTime << std::endl;
       auto& inputManager = Engine::GetInstance().GetInputManager();
       if(inputManager.IskeyPressed(GLFW_KEY_A))
       {
           std::cout <<"Key A is pressed"<< std::endl;
       }
    }

    void Game::Destroy()
    {
        std::cout <<"Game Destroyed"<< std::endl;
    }
 }