#include "Engine/Engine.h"
#include "Engine/scene/Scene.h"

#include "Engine/scene/Component/AnimationComponent.h"
#include "Engine/scene/Component/AudioComponent.h"
#include "Engine/scene/Component/AudioListenerComponent.h"
#include "Engine/scene/Component/CameraComponent.h"
#include "Engine/scene/Component/LightComponent.h"
#include "Engine/scene/Component/MeshComponent.h"
#include "Engine/scene/Component/PhysicsComponent.h"
#include "Engine/scene/Component/PlayerControllerComponent.h"

namespace GAMEDEV_ENGINE
{
    void Scene::RegisterTypes()
    {
        AnimationComponent::Register();
        AudioComponent::Register();
        AudioListenerComponent::Register();
        CameraComponent::Register();
        LightComponent::Register();
        MeshComponent::Register();
        PhysicsComponent::Register();
        PlayerControllerComponent::Register();
    }

    void Scene::Update(float deltaTime)
    {
        _mIsUpdating = true;
        for (auto objectIt = _mRootGameObjects.begin(); objectIt != _mRootGameObjects.end(); )
        {
            GameObject* object = objectIt->get();
            if (object->IsAlive())
            {
                printf("Updating: %s\n", object->GetName().c_str());
                fflush(stdout);
                object->Update(deltaTime);
                ++objectIt;
            }
            else
            {
                printf("Destroying: %s\n", object->GetName().c_str());
                fflush(stdout);
                objectIt = _mRootGameObjects.erase(objectIt);
            }
        }
        _mIsUpdating = false;

        // flush deferred additions
        for (auto& [obj, parent] : _mRootGameObjectsToAdd)
        {
            printf("Flushing deferred: %s\n", obj->GetName().c_str());
            fflush(stdout);
            _mRootGameObjects.emplace_back(obj);
            SetParent(obj, parent);
        }
        _mRootGameObjectsToAdd.clear();
    }

    void Scene::Clear()
    {
        _mRootGameObjects.clear();
    }

    GameObject* Scene::CreateGameObject(const std::string& name, GameObject* parent)
    {
        auto gameObject = new GameObject();
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

    GameObject* Scene::CreateGameObject(const std::string& type, const std::string& name, GameObject* parent)
    {
        auto gameObject = GameObjectFactory::GetInstance().CreateGameObject(type);
        if (gameObject)
        {
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
        }
        return gameObject;
    }

    bool Scene::SetParent(GameObject* obj, GameObject* parent)
    {
        bool result = false;
        auto currentParent = obj->GetParent();

        // ── CASE 1: Making obj a root object (parent == nullptr) ──────────────────
        if (parent == nullptr)
        {
            if (currentParent != nullptr)
            {
                // Move from currentParent->_mChildren to root
                auto it = std::find_if(
                    currentParent->_mChildren.begin(),
                    currentParent->_mChildren.end(),
                    [obj](const std::unique_ptr<GameObject>& child)
                    { return child.get() == obj; }
                );

                if (it != currentParent->_mChildren.end())
                {
                    _mRootGameObjects.emplace_back(std::move(*it));
                    obj->_mParent = nullptr;
                    currentParent->_mChildren.erase(it);
                    result = true;
                }
            }
            else
            {
                // No current parent — check if already in root
                auto it = std::find_if(
                    _mRootGameObjects.begin(),
                    _mRootGameObjects.end(),
                    [obj](const std::unique_ptr<GameObject>& child)
                    { return child.get() == obj; }
                );

                if (it == _mRootGameObjects.end())
                {
                    // Not in root yet — take ownership
                    _mRootGameObjects.emplace_back(obj);
                    obj->_mParent = nullptr;
                    result = true;
                }
                // else: already a root object — nothing to do
            }
        }

        // ── CASE 2: Assigning obj as child of parent ──────────────────────────────
        else
        {
            if (currentParent != nullptr)
            {
                // Move from currentParent->_mChildren to new parent->_mChildren
                auto it = std::find_if(
                    currentParent->_mChildren.begin(),
                    currentParent->_mChildren.end(),
                    [obj](const std::unique_ptr<GameObject>& child)
                    { return child.get() == obj; }
                );

                if (it != currentParent->_mChildren.end())
                {
                    // Cycle check
                    bool isCyclic = false;
                    auto tempParent = parent;
                    while (tempParent)
                    {
                        if (tempParent == obj)
                        {
                            isCyclic = true;
                            break;
                        }
                        tempParent = tempParent->GetParent();
                    }

                    if (!isCyclic)
                    {
                        parent->_mChildren.emplace_back(std::move(*it));
                        obj->_mParent = parent;
                        currentParent->_mChildren.erase(it);
                        result = true;
                    }
                }
            }
            else
            {
                // No current parent — search root list
                auto it = std::find_if(
                    _mRootGameObjects.begin(),
                    _mRootGameObjects.end(),
                    [obj](const std::unique_ptr<GameObject>& child)
                    { return child.get() == obj; }
                );

                if (it != _mRootGameObjects.end())
                {
                    // Found in root — move ownership to parent->_mChildren
                    parent->_mChildren.emplace_back(std::move(*it));
                    obj->_mParent = parent;
                    _mRootGameObjects.erase(it);
                    result = true;
                }
                // If not found anywhere — ownership is unknown, do nothing safely
            }
        }

        return result;
    }

    void Scene::SetMainCameraGameObject(GameObject* cameraGameObject)
    {
        _mMainCameraGameObject = cameraGameObject;
    }

    std::vector<LightData> Scene::CollectLights()
    {
        std::vector<LightData> lights;
        for (auto& obj : _mRootGameObjects)
        {
            CollectLightsRecursive(obj.get(), lights);
        }
        return lights;
    }

    void Scene::CollectLightsRecursive(GameObject* obj, std::vector<LightData>& out)
    {
        if (auto light = obj->GetComponent<LightComponent>())
        {
            LightData data;
            data.color = light->GetLightColor();
            data.position = obj->GetWorldPosition();
            out.push_back(data);
        }

        for (auto& child : obj->_mChildren)
        {
            CollectLightsRecursive(child.get(), out);
        }
    }

    std::shared_ptr<Scene> Scene::Load(const std::string& path)
    {
        const std::string contents = Engine::GetInstance().GetAssetFileSystem().LoadAssetFileText(path);
        if (contents.empty())
        {
            return nullptr;
        }

        auto jsonContent = nlohmann::json::parse(contents);
        if (jsonContent.empty())
        {
            return nullptr;
        }

        auto result = std::make_shared<Scene>();

        const std::string sceneName = jsonContent.value("name", "noname");

        if (jsonContent.contains("objects") && jsonContent["objects"].is_array())
        {
            const auto& objects = jsonContent["objects"];
            for (const auto& obj : objects)
            {
                result->LoadObject(obj, nullptr);
            }
        }

        if (jsonContent.contains("camera"))
        {
            std::string cameraObjName = jsonContent.value("camera", "");
            for (const auto& child : result->_mRootGameObjects)
            {
                // check root object itself first
                if (child->GetName() == cameraObjName)
                {
                    result->SetMainCameraGameObject(child.get());
                    break;
                }
                // then search children
                if (auto object = child->FindChildByName(cameraObjName))
                {
                    result->SetMainCameraGameObject(object);
                    break;
                }
            }
        }

        return result;
    }

    void Scene::LoadObject(const nlohmann::json& jsonObject, GameObject* parent)
    {
        const std::string name = jsonObject.value("name", "Object");
        GameObject* gameObject = nullptr;

        if (jsonObject.contains("type"))
        {
            const std::string type = jsonObject.value("type", "");
            if (type == "gltf")
            {
                std::string path = jsonObject.value("path", "");
                gameObject = GameObject::LoadGLTF(path, this);
                if (gameObject)
                {
                    gameObject->SetName(name);
                    if (parent)
                    {
                        gameObject->SetParent(parent);
                    }
                }
            }
            else
            {
                gameObject = CreateGameObject(type, name, parent);
            }
        }
        else
        {
            gameObject = CreateGameObject(name, parent);
        }

        if (!gameObject)
        {
            return;
        }

        // ── Transform ────────────────────────────────────────────────────────────

        if (jsonObject.contains("position"))
        {
            auto posObj = jsonObject["position"];
            glm::vec3 pos;
            pos.x = posObj.value("x", 0.0f);
            pos.y = posObj.value("y", 0.0f);
            pos.z = posObj.value("z", 0.0f);
            gameObject->SetPosition(pos);
        }

        if (jsonObject.contains("rotation"))
        {
            auto rotObj = jsonObject["rotation"];
            glm::quat rot;
            rot.x = rotObj.value("x", 0.0f);
            rot.y = rotObj.value("y", 0.0f);
            rot.z = rotObj.value("z", 0.0f);
            rot.w = rotObj.value("w", 1.0f);
            gameObject->SetRotation(rot);
        }

        if (jsonObject.contains("scale"))
        {
            auto scaleObj = jsonObject["scale"];
            glm::vec3 scale;
            scale.x = scaleObj.value("x", 1.0f);
            scale.y = scaleObj.value("y", 1.0f);
            scale.z = scaleObj.value("z", 1.0f);
            gameObject->SetScale(scale);
        }

        // ── Properties ───────────────────────────────────────────────────────────

        gameObject->LoadProperties(jsonObject);

        // ── Components ───────────────────────────────────────────────────────────

        if (jsonObject.contains("components") && jsonObject["components"].is_array())
        {
            const auto& components = jsonObject["components"];
            for (const auto& comp : components)
            {
                const std::string type = comp.value("type", "");
                Component* component = ComponentFactory::GetInstance().CreateComponent(type);
                if (component)
                {
                    component->LoadProperties(comp);
                    gameObject->AddComponent(component);
                }
            }
        }

        // ── Children ─────────────────────────────────────────────────────────────

        if (jsonObject.contains("children") && jsonObject["children"].is_array())
        {
            const auto& children = jsonObject["children"];
            for (const auto& child : children)
            {
                LoadObject(child, gameObject);
            }
        }

        // ── Init ─────────────────────────────────────────────────────────────────

        gameObject->Init();
    }
}