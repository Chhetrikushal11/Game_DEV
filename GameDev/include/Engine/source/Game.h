#pragma once
#include "Engine/source/eng.h"

namespace GAMEDEV_ENGINE 
{
    class Game: public Application
   {
        //
    public:
        bool Init() override;
        void Update(float deltaTime) override;
        void Destroy() override;
   }; 
}