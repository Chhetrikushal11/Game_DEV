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

		void Init() override;
		void Update(float deltaTime) override;

	private:
		std::shared_ptr<RigidBody> _mComponentRigidBody;


	};
}