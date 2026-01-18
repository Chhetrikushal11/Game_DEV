
#include "Engine/source/graphics/GraphicsAPI.h"
#include "Engine/source/render/RenderQueue.h"
#include "Engine/source/render/material.h"
#include "Engine/source/render/mesh.h"
namespace GAMEDEV_ENGINE
{
    void RenderQueue::Submit(const RenderCommand& command)
    {
        // inside submit we need to pushback the command to _mCommands
        _mCommands.push_back(command);
    }

    void RenderQueue::Draw(GraphicsAPI& graphicsAPI)
    {
        // in draw we iterate over all commands and for each commands 
        for (auto& command : _mCommands)
        {
            graphicsAPI.BindMaterial(command.material);
            graphicsAPI.BindMesh(command.mesh);
            graphicsAPI.DrawMesh(command.mesh);

            
        }

        // after we bind we can call the draw method
        _mCommands.clear();
    }

}