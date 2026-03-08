#pragma once
#include <cstddef>
#include <string>
#include <unordered_map>
#include <memory>

#include <nlohmann/json.hpp>

namespace GAMEDEV_ENGINE
{    

    class GameObject; // Forward declaration

    class Component
    {
    public:
        virtual ~Component() = default; // so that derived class destructors are called properly

        virtual void LoadProperties(const nlohmann::json& json);

        virtual void Update(float deltaTime) = 0; // Virtual update method for polymorphism

        // for init class
        virtual void Init();

        // Get the runtime type ID of this component instance
        virtual size_t GetTypeId() const = 0;

        // method to get the owner game object
        GameObject* GetOwner() const { return _mGameObjectOwner; }

        template<typename T>
        static size_t StaticTypeId()
        {
            static size_t typeID = _sNextTypeID++;
            return typeID;
        }

    protected:
        // handle the game object pointer in derived classes
        GameObject* _mGameObjectOwner = nullptr;
        // lets declare GameObject class as friend so that it can access protected and private members of Component class
        friend class GameObject;


    private:
        static size_t _sNextTypeID;  // Shared counter for all components 
        // unique identifier for component shared classes


    };

    // creating a factory to create a component with the type
    class ComponentCreatorBase
    {
    public:
        virtual ~ComponentCreatorBase() = default;
        virtual Component* CreateComponent() = 0;
    };

    template<typename T>
    class ComponentCreator :public ComponentCreatorBase
    {
        Component* CreateComponent() override
        {
            return new T();
        }
    };

    class ComponentFactory
    {
    public:
        //creating a  singleton
        static ComponentFactory& GetInstance();

        // method to register object
        template <typename T>
        void RegisterComponent(const std::string& name)
        {
            _mCreators.emplace(name, std::make_unique<ComponentCreator<T>>());
        }

        Component* CreateComponent(const std::string& name)
        {
            auto it = _mCreators.find(name);
            if (it != _mCreators.end())
            {
                return it->second->CreateComponent();
            }

            return nullptr;
        }
    private:
        std::unordered_map<std::string, std::unique_ptr<ComponentCreatorBase>> _mCreators;
    };
        // moving type ID to macro
#define COMPONENT(ComponentClass)\
public: \
    static size_t TypeId() { return GAMEDEV_ENGINE::Component::StaticTypeId<ComponentClass>(); }\
        size_t GetTypeId() const override { return TypeId(); }\
    static void Register() {GAMEDEV_ENGINE::ComponentFactory::GetInstance().RegisterComponent<ComponentClass>(std::string(#ComponentClass)); }

}