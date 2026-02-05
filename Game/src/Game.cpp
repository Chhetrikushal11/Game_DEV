
#include "Game.h"
#include "Engine/Application.h"
#include "Engine/graphics/GraphicsAPI.h"
#include "Engine/render/RenderQueue.h"
#include "Engine/scene/Component/CameraComponent.h"
#include "Engine/scene/Component/PlayerControllerComponent.h"
#include "TestObject.h"

// need to define stb image for its implementation
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
 namespace GAMEDEV_ENGINE 
 {
bool Game::Init()
{   // to access the file system
    auto& afs = Engine::GetInstance().GetAssetFileSystem();
    auto path = afs.GetAssetsFolder()/"textures/brick.png";
    int width, height, channels;

    unsigned char* data = stbi_load(path.string().c_str(), &width, &height, &channels, 0);
    std::shared_ptr<Texture> texture;
    if (data)
    {
        texture = std::make_shared<Texture>(width, height, channels, data);
        std::cout << "Image Loaded: " << width << "x" << height << " channels: " << channels << std::endl;
        std::cout << "Texture ID: " << texture->GetTextureID() << std::endl;  // Add this!
        stbi_image_free(data);
    }
    else { false; }
    
    // now need to intantiate the _mScene
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
        layout(location = 2) in vec2 uv;
        
        out vec3 vertexColor;
        out vec2 vUV;
        
        uniform mat4 uModel;
        uniform mat4 uView;
        uniform mat4 uProjection;
        
        void main()
        {
            gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
            vertexColor = aColor;
            vUV = uv;
        }
    )";

    // ✅ UPDATED FRAGMENT SHADER - with sine wave!
    std::string fragmentShader = R"(
        #version 330 core
        in vec3 vertexColor;
        in vec2 vUV;
        out vec4 FragColor;
        
        uniform float uTime;  // ✅ Time uniform
        uniform sampler2D brickTexture;
        
        void main()
        {   
             vec4 texColor = texture(brickTexture, vUV);
          /*  // Three different frequency sine waves
  
             float wave1 = sin(uTime * 1.0) * 0.5 + 0.5;
            float wave2 = sin(uTime * 2.0) * 0.5 + 0.5;
            float wave3 = sin(uTime * 3.0) * 0.5 + 0.5;
            
            // Method 1: Pulse brightness
            vec3 pulsedColor;
            pulsedColor.r = vertexColor.r * wave1;
            pulsedColor.g = vertexColor.g * wave2;
            pulsedColor.b = vertexColor.b * wave3; */
        
            FragColor = texColor * vec4(vertexColor, 1.0);
        }
    )";
    auto& graphicsAPI = GAMEDEV_ENGINE::Engine::GetInstance().GetGraphicsAPI();
    auto shaderProgram = graphicsAPI.CreateShaderProgram(vertexShader, fragmentShader);


    // after shader program initialization we need to set the shader program to material
    // we create material manually here
    auto _mMaterial = std::make_shared<GAMEDEV_ENGINE::Material>();
    _mMaterial->SetShaderProgram(shaderProgram);
    _mMaterial->SetTextureParams("brickTexture", texture);
    // this way mateiral is ready to be used in rendering

    // we using rectangle mesh for testing
    // rectangle with two triangles with positions and color per vertex
    std::vector<float> vertices = {
        // Position (3)      Color (3)         UV (2)

        // FRONT FACE (indices 0-3)
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,  // 0: top-right
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,  // 1: bottom-right
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,  // 2: bottom-left
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,  // 3: top-left

        // BACK FACE (indices 4-7)
         0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,  // 4: top-right
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,  // 5: bottom-right
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,  // 6: bottom-left
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 1.0f,  // 7: top-left

        // RIGHT FACE (indices 8-11)
         0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,  // 8: top-back
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,  // 9: bottom-back
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,  // 10: bottom-front
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,  // 11: top-front

         // LEFT FACE (indices 12-15)
         -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,  // 12: top-front
         -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,  // 13: bottom-front
         -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,  // 14: bottom-back
         -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,  // 15: top-back

         // TOP FACE (indices 16-19)
         -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,  // 16: back-left
          0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,  // 17: back-right
          0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,  // 18: front-right
         -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f,  // 19: front-left

         // BOTTOM FACE (indices 20-23)
         -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,  // 20: front-left
          0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,  // 21: front-right
          0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,  // 22: back-right
         -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f   // 23: back-left
    };

    std::vector<unsigned int> indices = {
        // FRONT FACE
        0, 1, 2,
        0, 2, 3,

        // BACK FACE
        4, 6, 5,
        4, 7, 6,

        // RIGHT FACE
        8, 9, 10,
        8, 10, 11,

        // LEFT FACE
        12, 13, 14,
        12, 14, 15,

        // TOP FACE
        16, 17, 18,
        16, 18, 19,

        // BOTTOM FACE
        20, 21, 22,
        20, 22, 23
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
        });

        // for UV
        vertexLayout.elements.push_back(
            {
                2,
                2,
                GL_FLOAT,
                sizeof(float) * 6
            }
    );

    vertexLayout.stride = sizeof(float) * 8;

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