#include "Engine/scene/GameObject.h"
#include <glm/gtc/matrix_transform.hpp> // for glm::translate, glm::rotate, glm::scale

namespace GAMEDEV_ENGINE
{
    void GameObject::Update(float deltaTime)
    {
        // iterate the _mComponents and call their update methods
        for (auto& component : _mComponents)
        {
            component->Update(deltaTime);
        
        }
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

    void GameObject::AddComponent(Component* component)
    {
        if(component)
        {
            _mComponents.emplace_back(component);
            component->_mGameObjectOwner = this; // setting the owner pointer
            // this can be access the component member as GameObject is declared as friend class in Component class

        }
    }

    void GameObject::MarkForDestroy()
    {
        if(_misAlive)
        {
            _misAlive = false;
        }
    }

    // for local transformation matrix
    glm::mat4 GameObject::GetLocalTransformMatrix() const
    {
        // this method combines translation, rotation, and scaling into a single transformation matrix

        // step 1 : Creating the identity matrix
        glm::mat4 identityMatrix = glm::mat4(1.0f); 
        
        // step 2: Translation first
        glm::mat4 translationMatrix = glm::translate(identityMatrix, _mPosition);

        // step 3: Rotation second (applying in ZYX order)
        
            // --- using quaternion --
        glm::mat4 rotationMatrix = glm::mat4_cast(_mRotation); // here glm::mat4_cast simply convert quat to mat4 for rotation

        //glm::mat4 rotationXMatrix = glm::rotate(identityMatrix, _mRotation.x, glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around X axis
        //glm::mat4 rotationYMatrix = glm::rotate(identityMatrix, _mRotation.y, glm::vec3(0.0f, 1.0f, 0.0f)); // rotate around X axis
        //glm::mat4 rotationZMatrix = glm::rotate(identityMatrix, _mRotation.z, glm::vec3(0.0f, 0.0f, 1.0f)); // rotate around X axis
        // combine rotations
       /* glm::mat4 rotationMatrix = rotationZMatrix * rotationYMatrix * rotationXMatrix;*/



        // step 4: Scaling last
        glm::mat4 scaleMatrix = glm::scale(identityMatrix, _mScale);

        // step 5: combine all transformations: Translation * Rotation * Scale  
        return translationMatrix * rotationMatrix * scaleMatrix;
    }   
    
    // for world transformation matrix
    glm::mat4 GameObject::GetWorldTransformMatrix() const
    {
        if(_mParent)
        {
            return _mParent->GetWorldTransformMatrix() * GetLocalTransformMatrix();
        }
        else
        {
            return GetLocalTransformMatrix();
        }   
        // this way every game object can be compute with respect to world scene rather than just local space
    } 

} // namespace GAMEDEV_ENGINE   