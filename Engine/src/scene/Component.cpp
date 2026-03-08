#include "Engine/scene/Component.h"



namespace GAMEDEV_ENGINE
{
    // Initialize static type ID counter
    size_t Component::_sNextTypeID = 1;

    void Component::LoadProperties(const nlohmann::json& json)
    {
    }

    void Component::Init()
    {

    }

    ComponentFactory& ComponentFactory::GetInstance()
    {
        static ComponentFactory instance;
        return instance;
    }
}
