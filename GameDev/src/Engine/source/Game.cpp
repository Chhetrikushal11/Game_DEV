#include <GL/glew.h>        // 1. GLEW FIRST!
#include <GLFW/glfw3.h>   

#include "Engine/source/Game.h"
#include "Engine/source/Application.h"
#include "Engine/source/graphics/GraphicsAPI.h"
#include "Engine/source/render/RenderQueue.h"

 namespace GAMEDEV_ENGINE 
 {
bool Game::Init()
{
    std::string vertexShader = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec3 aColor;

        out vec3 vertexColor;

        uniform vec2 uOffset;
        void main()
        {
            gl_Position = vec4(aPos.x + uOffset.x , aPos.y + uOffset.y, aPos.z, 1.0);
            vertexColor = aColor;
        }
    )";
    
    std::string fragmentShader = R"(
        #version 330 core
        in vec3 vertexColor;
        out vec4 FragColor;
        void main()
        {
            FragColor = vec4(vertexColor, 1.0);
        }
    )";
    
    auto& graphicsAPI = Engine::GetInstance().GetGraphicsAPI();
    auto shaderProgram = graphicsAPI.CreateShaderProgram(vertexShader, fragmentShader);
    // after shader program initialization we need to set the shader program to material
    _mMaterial.SetShaderProgram(shaderProgram);
    // this way mateiral is ready to be used in rendering

    // we using rectangle mesh for testing
    // rectangle with two triangles with positions and color per vertex
    std::vector<float> vertices = {
        // positions & colors
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,   // top right with red color
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom right with green color
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,   // bottom left with blue color
        -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f  // top left with yellow color
    };

    // now for indices
    std::vector<unsigned int> indices = {
        0, 1, 2, // first triangle
        0, 2, 3
    };

    // now creating vertex layout object and define the layout structure
    VertexLayout  vertexLayout;

    vertexLayout.elements.push_back(
        {
            0, // for position
            3, // number of position attribute
            GL_FLOAT, // data type
            0
        }
    );

    // for color
    vertexLayout.elements.push_back(
        {
            1,
            3, 
            GL_FLOAT,
            sizeof(float) * 3
        }
    );

    vertexLayout.stride = sizeof(float) * 6;

    _mMesh = std::make_unique<Mesh>(vertexLayout, vertices, indices);


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
           _mOffsetX -= 0.01f; // to move left
       } 

        else if(inputManager.IskeyPressed(GLFW_KEY_D))
        {
           _mOffsetX += 0.01f; // to move right
        }

        else if(inputManager.IskeyPressed(GLFW_KEY_W))
       {
           _mOffsetY += 0.01f; // to move upward
       } 

        else if(inputManager.IskeyPressed(GLFW_KEY_S))
        {
           _mOffsetY -= 0.01f; // to move downward
        }


        _mMaterial.SetFloatParams2f("uOffset", _mOffsetX, _mOffsetY);

       // to prepare render command
       RenderCommand command;
       command.material = &_mMaterial;
       command.mesh = _mMesh.get();

      auto& renderQueue = Engine::GetInstance().GetRenderQueue();
      renderQueue.Submit(command);

    }

    void Game::Destroy()
    {
        std::cout <<"Game Destroyed"<< std::endl;
    }
 }