#pragma once


#include "Engine/scene/Component.h"
#include "Engine/Physics/RigidBody.h"

namespace GAMEDEV_ENGINE
{
	class PhysicsComponent : public Component
	{
		COMPONENT(PhysicsComponent)
	public:
		PhysicsComponent() = default;
		PhysicsComponent(const std::shared_ptr<RigidBody>& body);
		void LoadProperties(const nlohmann::json & json) override;
		void Init() override;
		void Update(float deltaTime) override;

		// Setter 
		void SetRigidBody(const std::shared_ptr<RigidBody>& body);

	private:
		std::shared_ptr<RigidBody> _mComponentRigidBody;


	};
}