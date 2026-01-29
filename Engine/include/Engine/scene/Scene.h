#pragma once
#include <stdio.h>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>

#include "Engine/scene/GameObject.h"
namespace GAMEDEV_ENGINE
{
    class GameObject;
    class Scene
    {
        public:
        void Update(float deltaTime);
        void Clear();

        GameObject* CreateGameObject(const std::string& name, GameObject* parent = nullptr);

        // templated method to create game object of derived types
        template<typename T, typename = std::enable_if_t<std::is_base_of<GameObject, T>::value>>
        T* CreateGameObject(const std::string& name, GameObject* parent = nullptr)
        {
        auto gameObject = new T(); // create a new GameObject
        _mRootGameObjects.emplace_back(gameObject); // store it in the root game objects
        gameObject->SetName(name);
       // gameObject->SetParent(gameObject, parent);
        gameObject->SetParent(parent);
        return gameObject;

        }
        
        // method to set parent of a game object
        bool SetParent(GameObject* obj, GameObject* parent);
        // to set and get the camera game object
        void SetMainCameraGameObject(GameObject* cameraGameObject);
        GameObject* GetMainCameraGameObject() const {return _mMainCameraGameObject; };

        private:
    
            std::vector<std::unique_ptr<GameObject>> _mRootGameObjects;
            // creating a member for camera game object
            GameObject* _mMainCameraGameObject{nullptr};
    };

} // namespace GAMEDEV_ENGINE