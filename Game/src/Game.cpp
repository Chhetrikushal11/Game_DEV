
#include "Game.h"
#include "Engine/Application.h"
#include "Engine/graphics/GraphicsAPI.h"
#include "Engine/render/RenderQueue.h"
#include "Engine/scene/Component/CameraComponent.h"
#include "Engine/scene/Component/PlayerControllerComponent.h"
#include "TestObject.h"

 namespace GAMEDEV_ENGINE 
 {
bool Game::Init()
{   // now need to intantiate the _mScene
    _mScene = new Scene();
    // create a camera
    auto camera = _mScene->CreateGameObject("Camera");
    camera->AddComponent(new CameraComponent());
    camera->SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
    camera->AddComponent(new PlayerControllerComponent());
    // need to set main camera for the secene
    _mScene->SetMainCameraGameObject(camera);
    // if(!shaderProgram)  // If shader creation FAILED (nullptr)
    // {
    //     std::cerr << "Failed to create shader program" << std::endl;  // ✅ Error message
    //     return false;  // ✅ Return false (failure)
    // }
    // else  // If shader creation SUCCEEDED
    // {
    //     return true;  // ✅ Return true (success)
    // }
    if (!_mScene->CreateGameObject<TestObject>("TestObject1"))
    {
        std::cerr << "Failed to create TestObject1" << std::endl;
        return false;
    }

    std::cout << "TestObject1 created successfully" << std::endl;
    std::string vertexShader = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec3 aColor;
        
        out vec3 vertexColor;
        
        uniform mat4 uModel;
        uniform mat4 uView;
        uniform mat4 uProjection;
        
        void main()
        {
            gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
            vertexColor = aColor;
        }
    )";

    // ✅ UPDATED FRAGMENT SHADER - with sine wave!
    std::string fragmentShader = R"(
        #version 330 core
        in vec3 vertexColor;
        out vec4 FragColor;
        
        uniform float uTime;  // ✅ Time uniform
        
        void main()
        {
            // Three different frequency sine waves
            float wave1 = sin(uTime * 1.0) * 0.5 + 0.5;
            float wave2 = sin(uTime * 2.0) * 0.5 + 0.5;
            float wave3 = sin(uTime * 3.0) * 0.5 + 0.5;
            
            // Method 1: Pulse brightness
            vec3 pulsedColor;
            pulsedColor.r = vertexColor.r * wave1;
            pulsedColor.g = vertexColor.g * wave2;
            pulsedColor.b = vertexColor.b * wave3;
            
            FragColor = vec4(pulsedColor, 1.0);
        }
    )";
    auto& graphicsAPI = GAMEDEV_ENGINE::Engine::GetInstance().GetGraphicsAPI();
    auto shaderProgram = graphicsAPI.CreateShaderProgram(vertexShader, fragmentShader);
    // after shader program initialization we need to set the shader program to material
    // we create material manually here
    auto _mMaterial = std::make_shared<GAMEDEV_ENGINE::Material>();
    _mMaterial->SetShaderProgram(shaderProgram);

    // this way mateiral is ready to be used in rendering

    // we using rectangle mesh for testing
    // rectangle with two triangles with positions and color per vertex
    std::vector<float> vertices = {
        // positions & colors

        // front face
         0.5f,  0.5f, 0.5f, 1.0f, 0.0f, 0.0f,   // top right with red color
         0.5f, -0.5f, 0.5f,  0.0f, 1.0f, 0.0f,  // bottom right with green color
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,   // bottom left with blue color
        -0.5f,  0.5f, 0.5f, 1.0f, 1.0f, 0.0f,  // top left with yellow color

        //back face
         0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,   // top right with red color
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  // bottom right with green color
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,   // bottom left with blue color
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // top left with yellow color
    };

    // now for indices
    std::vector<unsigned int> indices = {
        //front face
        0, 1, 2,
        0, 2, 3,

        // top face
        4, 5, 1,
        4, 1, 0,

        // right side face
        4, 0, 3,
        4, 3, 7,

        // left side face

        1, 5,  6,
        1, 6, 2,

        // back face
        4, 7, 6,
        4, 6, 5,

        // bottom face
        3, 2, 6,
        3, 6, 7



    };

    // now creating vertex layout object and define the layout structure
    GAMEDEV_ENGINE::VertexLayout  vertexLayout;

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

    auto _mMesh = std::make_shared<GAMEDEV_ENGINE::Mesh>(vertexLayout, vertices, indices);
    // creating some object
    auto objectA = _mScene->CreateGameObject("ObjectA");
    objectA->AddComponent(new MeshComponent(_mMaterial, _mMesh));
    objectA->SetPosition(glm::vec3(0.0f, 2.0f, 0.0f));

    auto objectB  = _mScene->CreateGameObject("ObjectB");
    objectB->AddComponent(new MeshComponent(_mMaterial, _mMesh));
    objectB->SetPosition(glm::vec3(1.0f, 2.0f, 3.0f));
    //set rotation
    objectB->SetRotation(glm::vec3(0.0f, 1.57f, 0.0f));


    auto objectC = _mScene->CreateGameObject("ObjectC");
    objectC->AddComponent(new MeshComponent(_mMaterial, _mMesh));
    objectC->SetPosition(glm::vec3(-2.0f, 0.0f, 0.0f));
    objectC->SetRotation(glm::vec3(1.57f, 0.0f, 0.0f));
    objectC->SetScale(glm::vec3(1.5f, 1.5f, 1.5f));


    // Tell Engine about the scene
    Engine::GetInstance().SetCurrentScene(_mScene);

    std::cout << "Game Initialized" << std::endl;
    return true;
}


    void Game::Update(float deltaTime)
    {
       // std::cout <<"Game Updated with deltaTime: "<< deltaTime << std::endl;
        _mScene->Update(deltaTime);
    }

    void Game::Destroy()
    {
        std::cout <<"Game Destroyed"<< std::endl;
    }
 }