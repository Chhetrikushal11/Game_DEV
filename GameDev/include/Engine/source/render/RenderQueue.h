#pragma once
#include <vector>

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
    };


    class RenderQueue
    {
        public:
            // submitting method
            void Submit(const RenderCommand& command);

            void Draw(GraphicsAPI& graphicsAPI);
            
        private:
            // to hold the command we will create a container
            std::vector<RenderCommand> _mCommands;
    };
}