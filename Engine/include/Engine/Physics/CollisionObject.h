#pragma once

#include <vector>

#include "Engine/Common.h"


namespace GAMEDEV_ENGINE
{
	class IContactListener;

	enum class CollisionObjectType
	{
		RigidBody,
		KinematicCharacterController
	};

	class CollisionObject
	{
	public:
		CollisionObjectType GetCollisionObjectType();

		void AddContactListener(IContactListener* listener);
		void RemoveContactListener(IContactListener* listener);

	protected:
		void DispatchContactEvent(CollisionObject* obj,
			const glm::vec3& pos,
			const glm::vec3& norm);
		CollisionObjectType _mType;
		std::vector<IContactListener*> _mContactListeners;
		friend class PhysicsManager;
	};

	class IContactListener
	{
	public:
		virtual void OnContact(CollisionObject* obj,
			const glm::vec3& pos,
			const glm::vec3& norm) = 0;
	};
}

