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
        //template<typename T, typename = std::enable_if_t<std::is_base_of<GameObject, T>::value>>
        //T* CreateGameObject(const std::string& name, GameObject* parent = nullptr)
        //{
        //    auto gameObject = new T();
        //    _mRootGameObjects.emplace_back(gameObject);
        //    gameObject->SetName(name);
        //    gameObject->_mScene = this;

        //    if (_mIsUpdating)
        //    {
        //        _mRootGameObjectsToAdd.push_back({ gameObject, parent }); // ← gameObject not obj
        //    }
        //    else
        //    {
        //        SetParent(gameObject, parent);
        //    }

        //    return gameObject;
        //}

        template<typename T, typename = std::enable_if_t<std::is_base_of<GameObject, T>::value>>
        T* CreateGameObject(const std::string& name, GameObject* parent = nullptr)
        {
            auto gameObject = new T();
            gameObject->SetName(name);
            gameObject->_mScene = this;

            if (_mIsUpdating)
            {
                _mRootGameObjectsToAdd.push_back({ gameObject, parent });
            }
            else
            {
                _mRootGameObjects.emplace_back(gameObject);
                SetParent(gameObject, parent);
            }

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
            std::vector<std::pair<GameObject*, GameObject*>> _mRootGameObjectsToAdd;
            // creating a member for camera game object
            GameObject* _mMainCameraGameObject{nullptr};
            bool _mIsUpdating = false;
    };

} // namespace GAMEDEV_ENGINE