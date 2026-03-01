#pragma once
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <string>
#include <vector>
#include <memory>
#include "Engine/scene/Component.h"

namespace GAMEDEV_ENGINE
{
    class Scene;
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
        // setting bool value for parents
        bool SetParent(GameObject* parent);
        Scene* GetScene() { return _mScene; }

        // In GameObject.h under public:
        // void SetParent(GameObject* parent) { _mParent = parent; }
        // to check if object is alive
        bool IsAlive() const { return _misAlive; }
        // now making the marking for destroy
        void MarkForDestroy();
        
        // to manipulate transform later we can add methods here we add getters and setters for position, rotation, scale
        // ----------------------- for position ----------------------- //
        const glm::vec3& GetPosition() const { return _mPosition; }
        glm::vec3 GetWorldPosition();
        void SetPosition(const glm::vec3& position) { _mPosition = position; }
        // ----------------------- for rotation ----------------------- //
        const glm::quat& GetRotation() const { return _mRotation; }
        void SetRotation(const glm::quat& rotation) { _mRotation = rotation; }
        // ----------------------- for scaling ----------------------- //
        const glm::vec3& GetScale() const { return _mScale; }
        void SetScale(const glm::vec3& scale) { _mScale = scale; }
        // now to compute the transformation matrix
        glm::mat4 GetLocalTransformMatrix() const;
        glm::mat4 GetWorldTransformMatrix() const;
        // Adding the component management methods
        void AddComponent(Component* component);

        // for loading GLTF
        static GameObject* LoadGLTF(const std::string& path);

        
        // for isActive
        void SetActive(bool active);
        bool IsActive() const { return _mActive; }
        
        // ✅ OPTIMIZED: GetComponent using TypeID (O(n) but faster than dynamic_cast)
        template<typename T, typename = std::enable_if_t<std::is_base_of_v<Component, T>>>
        T* GetComponent()
        {
            // Get the unique type ID for component type T
            size_t typeId = Component::StaticTypeId<T>();

            // Search through components
            for (auto& component : _mComponents)
            {
                if (component->GetTypeId() == typeId)
                {
                    return static_cast<T*>(component.get());  // Safe: we know the type matches
                }
            }
            return nullptr;
        }

        // to find child by name
        GameObject* FindChildByName(const std::string& name);
        
    protected:
        // to avoid direct instantiation
        GameObject() = default;
        
    protected:
        std::string _mName;
        GameObject* _mParent{nullptr};
        Scene* _mScene{ nullptr };
        // need to add children vector
        std::vector<std::unique_ptr<GameObject>> _mChildren;
        // need to create a contianer for components
        std::vector<std::unique_ptr<Component>> _mComponents;
        bool _misAlive{true};
        // for transform we can add position, rotation, scale later
        glm::vec3 _mPosition{0.0f, 0.0f, 0.0f};
        glm::quat _mRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // identity quaternian
        glm::vec3 _mScale{1.0f, 1.0f, 1.0f};
        bool _mActive = true;
        // declaring scene as friend class to access private members
        friend class Scene;
    };



} // namespace GAMEDEV_ENGINE