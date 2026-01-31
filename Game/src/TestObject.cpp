#include <glm/mat4x4.hpp>
#include "TestObject.h"
#include "Engine/scene/Component/MeshComponent.h"

// implementing the constructor
TestObject::TestObject()
{
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
    auto& graphicsAPI =GAMEDEV_ENGINE::Engine::GetInstance().GetGraphicsAPI();
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
    // adding new mesh component to the TestObject
    AddComponent(new GAMEDEV_ENGINE::MeshComponent(_mMaterial, _mMesh));


}

void TestObject::Update(float deltaTime)
{
    GAMEDEV_ENGINE::GameObject::Update(deltaTime);
    // std::cout <<"Current deltaTime: "<< deltaTime << std::endl;
#if 0
    // first we get the position
    auto position = GetPosition();
    // updating offset based on position
    auto& inputManager = GAMEDEV_ENGINE::Engine::GetInstance().GetInputManager();
    if (inputManager.IskeyPressed(GLFW_KEY_A))
    {
        position.x -= 0.01f; // to move left
    }
    if (inputManager.IskeyPressed(GLFW_KEY_D))
    {
        position.x += 0.01f; // to move right
    }
    if (inputManager.IskeyPressed(GLFW_KEY_W))
    {
        position.y += 0.01f; // to move upward
    }
    if (inputManager.IskeyPressed(GLFW_KEY_S))
    {
        position.y -= 0.01f; // to move downward
    }
    if (inputManager.IskeyPressed(GLFW_KEY_H))
    {
        position = glm::vec3(0.0f, 0.0f, 0.0f);  // Reset to origin
    }

    SetPosition(position);
#endif
        _mTime += deltaTime; // to accumulate time


        // _mMaterial.SetFloatParams2f("uOffset",position.x, position.y); // setting offset uniform
        // _mMaterial.SetFloatParams("uTime", _mTime); // this ensures time is updated every frame
            auto meshComponent = GetComponent<GAMEDEV_ENGINE::MeshComponent>();
    if(meshComponent)
    {
        auto material = meshComponent->GetMaterial();
        if(material)
        {
            material->SetFloatParams("uTime", _mTime);
        }
    }

}

TestObject::~TestObject()
{
       // destructor implementation (if needed)
}
