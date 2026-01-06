#pragma once


 namespace GAMEDEV_ENGINE 
 {
    class Application
    {
        public:
            Application();
            ~Application();

            virtual  bool Init();
            virtual void Update(float deltaTime);
            virtual void Destroy();
            void SetNeedsToClosed(bool needsToClose);    
            bool NeedsToClosed() const;
        private:
            bool _mNeedsToClose;
    };
 }