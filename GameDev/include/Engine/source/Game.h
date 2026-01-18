#pragma once
#include <memory>

#include "Engine/source/eng.h"  // ✅ Use eng.h
#include "Engine/source/Application.h"



namespace GAMEDEV_ENGINE 
{
    class Game: public Application
   {
        //
    public:
        bool Init() override;
        void Update(float deltaTime) override;
        void Destroy() override;
    private:
        // adding material 
        Material _mMaterial;
        std::unique_ptr<Mesh> _mMesh; 
        // we create a unique pointer to Mesh because mesh is heavy object and we dont want to copy0 it
        float _mOffsetX = 0.0f;
        float _mOffsetY = 0.0f;
    }; 
}