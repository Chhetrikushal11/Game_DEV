#include "JumpPlatform.h"

namespace GAMEDEV_ENGINE
{
	void JumpPlatform::Init()
	{
		auto physics = GetComponent<PhysicsComponent>();
		if (physics)
		{
			auto rigidBody = physics->GetRigidBody();
			if (rigidBody)
			{
				rigidBody->AddContactListener(this);
			}
		}
	}
	void JumpPlatform::OnContact(CollisionObject* obj, const glm::vec3& pos, const glm::vec3& norm)
	{
		if (obj->GetCollisionObjectType() == CollisionObjectType::KinematicCharacterController)
		{
			auto controller = static_cast<KinematicsCharacterController*>(obj);
			if (controller)
			{
				controller->Jump(glm::vec3(0.0f, 20.0f, 0.0f));
			}
		}
	}
}