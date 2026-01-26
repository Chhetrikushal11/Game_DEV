#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

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
        // In GameObject.h under public:
        void SetParent(GameObject* parent) { _mParent = parent; }

        // to check if object is alive
        bool IsAlive() const { return _misAlive; }

        // now making the marking for destroy
        void MarkForDestroy();
        
        // to manipulate transform later we can add methods here we add getters and setters for position, rotation, scale
        // ----------------------- for position ----------------------- //
        const glm::vec3& GetPosition() const { return _mPosition; }
        void SetPosition(const glm::vec3& position) { _mPosition = position; }

        // ----------------------- for rotation ----------------------- //
        const glm::vec3& GetRotation() const { return _mRotation; }
        void SetRotation(const glm::vec3& rotation) { _mRotation = rotation; }

        // ----------------------- for scaling ----------------------- //
        const glm::vec3& GetScale() const { return _mScale; }
        void SetScale(const glm::vec3& scale) { _mScale = scale; }

        // now to compute the transformation matrix
        glm::mat4 GetLocalTransformMatrix() const;
        glm::mat4 GetWorldTransformMatrix() const;

        protected:
            // to avoid direct instantiation
            GameObject() = default;
        private:
            std::string _mName;
            GameObject* _mParent{nullptr};
            // need to add children vector
            std::vector<std::unique_ptr<GameObject>> _mChildren;
            bool _misAlive{true};
            // for transform we can add position, rotation, scale later
            glm::vec3 _mPosition{0.0f, 0.0f, 0.0f};
            glm::vec3 _mRotation{0.0f, 0.0f, 0.0f};
            glm::vec3 _mScale{1.0f, 1.0f, 1.0f};
            // declaring scene as friend class to access private members
            friend class Scene;


    };
} // namespace GAMEDEV_ENGINE