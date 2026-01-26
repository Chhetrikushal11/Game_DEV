#pragma once
#include <string>
#include <vector>
#include <memory>

namespace GAMEDEV_ENGINE
{
    class GameObject
    {
        public:
        // we we pass destrcutor as virtual 
        virtual ~GameObject() = default;
        // to prevent direct instantiation
        // virtal destructor to allow proper cleanup in derived classes
        // virutal destructor is important in base classes to ensure derived class destructors are called correctly

        // using virutal update method for polymorphism
        virtual void Update(float deltaTime);

        // getting and setting name
        const std::string& GetName() const { return _mName; }
        void SetName(const std::string& name);


        // getting and setting parent
        GameObject* GetParent() const { return _mParent; }
        // void SetParent(GameObject* parent); 

        // to check if object is alive
        bool IsAlive() const { return _misAlive; }

        // now making the marking for destroy
        void MarkForDestroy();

        protected:
            // to avoid direct instantiation
            GameObject() = default;
        private:
            std::string _mName;
            GameObject* _mParent{nullptr};
            // need to add children vector
            std::vector<std::unique_ptr<GameObject>> _mChildren;
            bool _misAlive{true};
            
            // declaring scene as friend class to access private members
            friend class Scene;


    };
} // namespace GAMEDEV_ENGINE