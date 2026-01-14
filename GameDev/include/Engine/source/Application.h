#pragma once

namespace GAMEDEV_ENGINE
{
    class Application
    {
    public:
        Application();
        virtual ~Application();
        
        virtual bool Init() = 0;
        virtual void Update(float deltaTime) = 0;
        virtual void Destroy() = 0;
        
        void SetNeedsToClosed(bool needsToClose);    
        bool NeedsToClosed() const;
        
    private:
        bool _mNeedsToClose;
    };
}