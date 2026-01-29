#pragma once
#include <vector>
#include <glm/mat4x4.hpp>

namespace GAMEDEV_ENGINE
{
    /*
        Inside the RenderQueue we will save the draw command.

     */

     // lets create a struct to save draw command
    class Material;
    class Mesh;
    class GraphicsAPI;
    
    struct RenderCommand
    {
        Mesh* mesh = nullptr; // to get the mesh data
        Material* material = nullptr;
        glm::mat4 modelMatrix = glm::mat4(1.0f); // this will store world transform for each object
    };

    // CREATING STURCT DATA for CAMERA
    struct CameraData
    {
        glm::mat4 viewMatrix = glm::mat4(1.0f);
        glm::mat4 projectionMatrix = glm::mat4(1.0f);
    };

    class RenderQueue
    {
        public:
            // submitting method
            void Submit(const RenderCommand& command);

            void Draw(GraphicsAPI& graphicsAPI, CameraData& cameraData);
            
        private:
            // to hold the command we will create a container
            std::vector<RenderCommand> _mCommands;
    };
}