#include "Engine/scene/Scene.h"
#include "Engine/scene/Component/LightComponent.h"



namespace GAMEDEV_ENGINE
{
    void Scene::Update(float deltaTime)
     {
        // here we iterate over children containers and call their update methods
        for (auto objectIt = _mRootGameObjects.begin(); objectIt != _mRootGameObjects.end(); )
        {
            GameObject* object = objectIt->get();
            if(object->IsAlive())
            {
            object->Update(deltaTime);
                ++objectIt;
            }
            else
            {
                // remove the child from the children vector
                objectIt = _mRootGameObjects.erase(objectIt);
            }
        }
    }

    // to clear the scene
    void Scene::Clear()
    {
        _mRootGameObjects.clear();
    }

    // to create a game object
    GameObject* Scene::CreateGameObject(const std::string& name, GameObject* parent)
    {
        auto gameObject = new GameObject(); // create a new GameObject
        _mRootGameObjects.emplace_back(gameObject); // store it in the root game objects
        gameObject->SetName(name);
        gameObject->_mScene = this;
        SetParent(gameObject, parent);
        return gameObject;

        // auto gameObject = std::make_unique<GameObject>();
        // gameObject->SetName(name);
        // gameObject->SetParent(parent);
        // return gameObject.get();
    }

    // setting up the parent of a game object
    bool Scene::SetParent(GameObject* obj, GameObject* parent)
    {

        bool result = false;
        auto currentParent = obj->GetParent(); // this gives us the current parent of the object we want to reassign

        // case  1: if the current parent is null
        if(parent == nullptr)
        {
            // this means we want to set the object as a root game object
                // sub case  1: if the object already has a parent
                if(currentParent != nullptr)
                {
                    // we need to add the object to root list and remove it from the current parent's children list
                    // first we remove it from the current parent's children list
                    auto it = std::find_if(
                        currentParent->_mChildren.begin(),
                        currentParent->_mChildren.end(),
                        [obj](const std::unique_ptr<GameObject>& child)
                        { return child.get() == obj; }   
                    );

                    if(it != currentParent->_mChildren.end())
                    {
                        _mRootGameObjects.emplace_back(std::move(*it));
                        // using emplace_back to move the unique ptr to root game objects rather than using push_back
                        // as push_back would create a copy which is not allowed for unique_ptr
                        obj->_mParent = nullptr; // setting the parent to null
                        currentParent->_mChildren.erase(it);
                        result = true;
                    }


                }

                // sub case 2: if the object has no parent currently
                   /*
                               this can happen in two cases:
                                 1. when the object is already a root game object
                                 2. when the object is newly created and has no parent

                    */

                else
                {
                  // first we will iterate to check if the object is already in the root game objects list
                  // rather than searching it in _mChildren list we will check in root list
                        auto it = std::find_if(
                        _mRootGameObjects.begin(),
                        _mRootGameObjects.end(),
                        [obj](const std::unique_ptr<GameObject>& child)
                        { return child.get() == obj; }   
                    );

                    if(it == _mRootGameObjects.end())
                    {
                        // if not found we add it to root game objects
                        std::unique_ptr<GameObject> tempPtr(obj);
                        _mRootGameObjects.emplace_back(std::move(tempPtr));
                        // here we create a temporary unique ptr to transfer the ownership to root game objects
                        obj->_mParent = parent;
                        result = true;
                    }

                }

        }
        // case 2: if the parent is not null
        // we are trying to set a a child of another object
        else
        {
            // case 1: if the object already has a parent
            if(currentParent != nullptr)
            {
                // first locate the object in the current parent's children list
                auto it = std::find_if(
                    currentParent->_mChildren.begin(),
                    currentParent->_mChildren.end(),
                    [obj](const std::unique_ptr<GameObject>& child)
                    { return child.get() == obj; }   
                );

                if(it != currentParent->_mChildren.end())
                {
                    // we need to ensure there is no cyclic dependency
                    // should prevent siging an object as a child of its own descendant
                    bool isCyclic = false;
                    auto tempParent = parent;
                    while(tempParent)
                    {
                        if(tempParent == obj) // this means we have a cyclic dependency
                        {
                            isCyclic = true;
                            break;
                        }
                        tempParent = tempParent->GetParent();
                    }

                    if(!isCyclic)
                    {
                        // now we can safely reassign the parent
                        parent->_mChildren.emplace_back(std::move(*it));
                        obj->_mParent = parent;
                        currentParent->_mChildren.erase(it);
                        result = true;
                    }   

                }   

            }

            else
            {
                // if the current parent is null
                /*
                            this can happen in two cases:
                                 1. when the object is already a root game object
                                 2. when the object is newly created and has no parent
                */
               // first search it in same route
               auto it = std::find_if(
                    _mRootGameObjects.begin(),
                    _mRootGameObjects.end(),
                    [obj](const std::unique_ptr<GameObject>& child)
                    { return child.get() == obj; }   
                );
                // if the object is found in root game objects
                if(it == _mRootGameObjects.end())
                {
                    // the object is just a newly created object
                    std::unique_ptr<GameObject> tempPtr(obj); // create a temporary unique ptr to transfer ownership
                    // here we create a temporary unique ptr to transfer the ownership to parent's children list
                    parent->_mChildren.emplace_back(std::move(tempPtr));
                    /*obj->_mParent = parent;*/
                    result = true;
                }
                else
                {
                    // if found we move it from root game objects to parent's children list
                    parent->_mChildren.emplace_back(std::move(*it));
                    obj->_mParent = parent;
                    _mRootGameObjects.erase(it);
                    result = true;
                }


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
        // here it will recurssively check for the light component in each component. 
        // if object have light data grab its color in world position.
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

}