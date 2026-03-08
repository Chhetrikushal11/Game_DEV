#pragma once
#include <memory>

#include "Engine/eng.h"  // ✅ Use eng.h
#include "Engine/Application.h"
#include "Engine/scene/Scene.h"



namespace GAMEDEV_ENGINE 
{
    class Game: public Application
   {
        //
    public:
        void RegisterTypes() override;
        bool Init() override;
        void Update(float deltaTime) override;
        void Destroy() override;
    private:
      std::shared_ptr<Scene> _mScene ;
 
    }; 
}