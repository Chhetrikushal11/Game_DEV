#pragma once
#include <stdio.h>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>

#include <nlohmann/json.hpp>

#include "Engine/scene/GameObject.h"
#include "Engine/Common.h"
namespace GAMEDEV_ENGINE
{
    class GameObject;
    class Scene
    {
        public:
            // static method
        static void RegisterTypes();
        void Update(float deltaTime);
        void Clear();

        GameObject* CreateGameObject(const std::string& name, GameObject* parent = nullptr);
        
        // Creating Object using type
        GameObject* CreateGameObject(const std::string& type, const std::string& name, GameObject* parent = nullptr);

        // templated method to create game object of derived types
        template<typename T, typename = std::enable_if_t<std::is_base_of<GameObject, T>::value>>
        T* CreateGameObject(const std::string& name, GameObject* parent = nullptr)
        {
        auto gameObject = new T(); // create a new GameObject
        _mRootGameObjects.emplace_back(gameObject); // store it in the root game objects
        gameObject->SetName(name);
       // gameObject->SetParent(gameObject, parent);
        gameObject->_mScene = this;
        gameObject->SetParent(parent);
        
        return gameObject;

        }
        
        // method to set parent of a game object
        bool SetParent(GameObject* jsonObj, GameObject* parent);
        // to set and get the camera game object
        void SetMainCameraGameObject(GameObject* cameraGameObject);
        GameObject* GetMainCameraGameObject() const {return _mMainCameraGameObject; };
        // to get and set the light data
        std::vector<LightData> CollectLights();

        // to load the whole scene
        static std::shared_ptr<Scene> Load(const std::string& path);

        // for recurssive function we add private helper
        private:
            void CollectLightsRecursive(GameObject* obj, std::vector<LightData>& out);
            // now to load whole json file
            void LoadObject(const nlohmann::json& object, GameObject* parent);

        private:
    
            std::vector<std::unique_ptr<GameObject>> _mRootGameObjects;
            // creating a member for camera game object
            GameObject* _mMainCameraGameObject{nullptr};
    };

} // namespace GAMEDEV_ENGINE