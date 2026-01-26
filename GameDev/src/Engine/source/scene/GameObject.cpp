#include "Engine/source/scene/GameObject.h"

namespace GAMEDEV_ENGINE
{
    void GameObject::Update(float deltaTime)
    {
        // here we iterate over children containers and call their update methods
        for (auto childrenIt = _mChildren.begin(); childrenIt != _mChildren.end(); )
        {
            GameObject* child = childrenIt->get();
            if(child->IsAlive())
            {
                child->Update(deltaTime);
                ++childrenIt;
            }
            else
            {
                // remove the child from the children vector
                childrenIt = _mChildren.erase(childrenIt);
            }
        }
    }

    void GameObject::SetName(const std::string& name)
    {
        _mName = name;
    }

    // void GameObject::SetParent(GameObject* parent)
    // {
    //     _mParent = parent;
    // }

    void GameObject::MarkForDestroy()
    {
        if(_misAlive)
        {
            _misAlive = false;
        }
    }

} // namespace GAMEDEV_ENGINE   