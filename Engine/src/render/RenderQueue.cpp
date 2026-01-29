
#include "Engine/graphics/GraphicsAPI.h"
#include "Engine/render/RenderQueue.h"
#include "Engine/render/material.h"
#include "Engine/render/mesh.h"
#include "Engine/graphics/ShaderProgram.h"
namespace GAMEDEV_ENGINE
{
    void RenderQueue::Submit(const RenderCommand& command)
    {
        // inside submit we need to pushback the command to _mCommands
        _mCommands.push_back(command);
    }

    void RenderQueue::Draw(GraphicsAPI& graphicsAPI, CameraData& cameraData)
    {
        // in draw we iterate over all commands and for each commands 
        for (auto& command : _mCommands)
        {
            graphicsAPI.BindMaterial(command.material);
            // here we get the Shader progarm from the material and Set the model matrix uniform
            command.material->GetShaderProgram()->SetUniformMat4f("uModel", command.modelMatrix);
            // need to pass camera and view projection matrix to the shader
            command.material->GetShaderProgram()->SetUniformMat4f("uView", cameraData.viewMatrix);
            command.material->GetShaderProgram()->SetUniformMat4f("uProjection", cameraData.projectionMatrix);
            graphicsAPI.BindMesh(command.mesh);
            graphicsAPI.DrawMesh(command.mesh);

            
        }

        // after we bind we can call the draw method
        _mCommands.clear();
    }

}