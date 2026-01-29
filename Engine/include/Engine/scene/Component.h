#pragma once

namespace GAMEDEV_ENGINE
{
    class GameObject; // Forward declaration

    class Component
    {
        public:
            virtual ~Component() = default; // so that derived class destructors are called properly

            virtual void Update(float deltaTime); // Virtual update method for polymorphism
        
            // method to get the owner game object
            GameObject* GetOwner() const { return _mGameObjectOwner; }
        protected:
        // handle the game object pointer in derived classes
            GameObject* _mGameObjectOwner = nullptr;
            // lets declare GameObject class as friend so that it can access protected and private members of Component class
            friend class GameObject;
            

        private:
    };
           
}