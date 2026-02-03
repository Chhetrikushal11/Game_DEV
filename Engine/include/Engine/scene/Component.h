#pragma once
#include <cstddef> // for size_t
namespace GAMEDEV_ENGINE
{
    class GameObject; // Forward declaration

    class Component
    {
        public:
            virtual ~Component() = default; // so that derived class destructors are called properly

            virtual void Update(float deltaTime) = 0; // Virtual update method for polymorphism


            // Get the runtime type ID of this component instance
            virtual size_t GetTypeId() const = 0;
        
            // method to get the owner game object
            GameObject* GetOwner() const { return _mGameObjectOwner; }

            template<typename T>
            static size_t StaticTypeId()
            {
                static size_t typeID = _sNextTypeID++;
                    return typeID;
            }

        protected:
        // handle the game object pointer in derived classes
            GameObject* _mGameObjectOwner = nullptr;
            // lets declare GameObject class as friend so that it can access protected and private members of Component class
            friend class GameObject;
            

        private:
            static size_t _sNextTypeID;  // Shared counter for all components 
            // unique identifier for component shared classes


    };
    // moving type ID to macro
#define COMPONENT(ComponentClass)\
public: \
    static size_t TypeId() { return Component::StaticTypeId<ComponentClass>(); }\
        size_t GetTypeId() const override { return TypeId(); }

           
}