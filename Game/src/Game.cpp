
#include "Game.h"
#include "Player/Player.h"
#include "Engine/Application.h"
#include "Engine/graphics/GraphicsAPI.h"
#include "Engine/render/RenderQueue.h"
#include "Engine/scene/Component/CameraComponent.h"
#include "Engine/scene/Component/LightComponent.h"
#include "Engine/scene/Component/PlayerControllerComponent.h"
#include "Engine/scene/Component/PhysicsComponent.h"
#include "Engine/Physics/RigidBody.h"
#include "TestObject.h"


 namespace GAMEDEV_ENGINE 
 {
bool Game::Init()
{   // to access the file system
    auto& afs = Engine::GetInstance().GetAssetFileSystem();
  
    auto texture= Texture::Load("textures/brick.png");

    
    // now need to intantiate the _mScene
    _mScene = new Scene();

    // Tell Engine about the scene
    Engine::GetInstance().SetCurrentScene(_mScene);
    // create a camera
    //auto camera = _mScene->CreateGameObject("Camera");
    //camera->AddComponent(new CameraComponent());
    //camera->SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
    //camera->AddComponent(new PlayerControllerComponent());
    // need to set main camera for the secene
    auto player = _mScene->CreateGameObject<Player>("Player");
    player->Init();
   _mScene->SetMainCameraGameObject(player);
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
    std::string vertexShader = afs.LoadAssetFileText("shaders/vertex.glsl");
    std::string fragmentShader = afs.LoadAssetFileText("shaders/fragment.glsl");
    //  std::string vertexShader = R"(
    // 
    // 
    //std::string vertexShader = R"(
    //    #version 330 core
    //    layout(location = 0) in vec3 aPos;
    //    layout(location = 1) in vec3 aColor;
    //    layout(location = 2) in vec2 uv;
    //    
    //    out vec3 vertexColor;
    //    out vec2 vUV;
    //    
    //    uniform mat4 uModel;
    //    uniform mat4 uView;
    //    uniform mat4 uProjection;
    //    
    //    void main()
    //    {
    //        gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
    //        vertexColor = aColor;
    //        vUV = uv;
    //    }
    //)";

  //  // ✅ UPDATED FRAGMENT SHADER - with sine wave!
  //  std::string fragmentShader = R"(
  //      #version 330 core
  //      in vec3 vertexColor;
  //      in vec2 vUV;
  //      out vec4 FragColor;
  //      
  //      uniform float uTime;  // ✅ Time uniform
  //      uniform sampler2D brickTexture;
  //      
  //      void main()
  //      {   
  //           vec4 texColor = texture(brickTexture, vUV);
  //        /*  // Three different frequency sine waves
  //
  //           float wave1 = sin(uTime * 1.0) * 0.5 + 0.5;
  //          float wave2 = sin(uTime * 2.0) * 0.5 + 0.5;
  //          float wave3 = sin(uTime * 3.0) * 0.5 + 0.5;
  //          
  //          // Method 1: Pulse brightness
  //          vec3 pulsedColor;
  //          pulsedColor.r = vertexColor.r * wave1;
  //          pulsedColor.g = vertexColor.g * wave2;
  //          pulsedColor.b = vertexColor.b * wave3; */
  //      
  //          FragColor = texColor * vec4(vertexColor, 1.0);
  //      }
  //)";
   

    // after shader program initialization we need to set the shader program to material
    // we create material manually here
    auto _mMaterial = Material::Load("materials/brick.mat");
    auto _mMesh = Mesh::CreateBox();
    // this way mateiral is ready to be used in rendering

    // we using rectangle mesh for testing
    // rectangle with two triangles with positions and color per vertex
  

    
    // creating some object
    auto objectA = _mScene->CreateGameObject("ObjectA");
    objectA->AddComponent(new MeshComponent(_mMaterial, _mMesh));
    objectA->SetPosition(glm::vec3(1.0f, 0.0f, -5.0f));

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


    // Load Suzanne mesh from GLTF
    //auto suzanneMesh = Mesh::Load("models/Suzanne.gltf");
    //if (!suzanneMesh)
    //{
    //    std::cerr << "Failed to load Suzanne mesh!" << std::endl;
    //    return false;
    //}
    //std::cout << "Suzanne mesh loaded successfully!" << std::endl;

    //// Load Suzanne material
    //auto suzanneMaterial = Material::Load("materials/suzanne.mat");
    //if (!suzanneMaterial)
    //{
    //    std::cerr << "Failed to load Suzanne material!" << std::endl;
    //    return false;
    //}
    //std::cout << "Suzanne material loaded successfully!" << std::endl;

  
    // Create Suzanne game object
    auto suzanneObj = GameObject::LoadGLTF("models/Suzanne/Suzanne.gltf");
  /*  suzanneObj->AddComponent(new MeshComponent(suzanneMaterial, suzanneMesh)); */
    suzanneObj->SetPosition(glm::vec3(-5.0f, 0.0f, 0.0f));  // Move to the left

    // for the FPS
    //auto gunObj = GameObject::LoadGLTF("models/sten_gunmachine_carbine/scene.gltf");
    //// we will attach weapon as child object of the camera
    //gunObj->SetParent(camera);
    //gunObj->SetPosition(glm::vec3(0.75f, -0.5f, -0.75f));
    //gunObj->SetScale(glm::vec3(-1.0f, 1.0f, 1.0f));

    //if (auto anim = gunObj->GetComponent<AnimationComponent>())
    //{
    //    if (auto bullet = gunObj->FindChildByName("bullet_33"))
    //    {
    //       bullet->SetActive(false);
    //    }

    //    if (auto fire = gunObj->FindChildByName("BOOM_35"))
    //    {
    //        fire->SetActive(false);
    //    }
 
    //  anim->Play("shoot");

    //}

    // create a light objce
    auto lightObj = _mScene->CreateGameObject("Light");
    auto lightComp =  new LightComponent();
    lightComp->SetLightColor(glm::vec3(1.0f, 1.0f,1.0f)); // for white color
    // add the light component to gameobject
    lightObj->AddComponent(lightComp);
    lightObj->SetPosition(glm::vec3(0.0f, 5.0f, 0.0f));
    
    // create ground object
    auto groundObj = _mScene->CreateGameObject("Ground");
    groundObj->SetPosition(glm::vec3(0.0f, -5.0f, 0.0f));

    glm::vec3 groundExtents(20.0f, 2.0f, 20.0f);
    auto groundMesh = Mesh::CreateBox(groundExtents);
    groundObj->AddComponent(new MeshComponent(_mMaterial, groundMesh));

    auto groundCollider = std::make_shared<BoxCollider>(groundExtents);
    auto groundBody = std::make_shared<RigidBody>(BodyType::Static, groundCollider, 0.0f, 0.5f);
    groundObj->AddComponent(new PhysicsComponent(groundBody));

    auto boxObj = _mScene->CreateGameObject("FailingBox");
    boxObj->AddComponent(new MeshComponent(_mMaterial, _mMesh));
    boxObj->SetPosition(glm::vec3(0.0f, 2.0f, 2.0f));
    boxObj->SetRotation(glm::quat(glm::vec3(1.0f, 2.0f, 0.0f)));
    auto boxCollider = std::make_shared<BoxCollider>(glm::vec3(1.0f));
    auto boxBody = std::make_shared<RigidBody>(
        BodyType::Dynamic, boxCollider, 5.0f, 0.5f);
    boxObj->AddComponent(new PhysicsComponent(boxBody));

   //  player->SetPosition(glm::vec3(0.0f, 1.0f, 7.0f));
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