#pragma once
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

#include<memory>
#include "Engine/Physics/Collider.h"


class btRigidBody;

namespace GAMEDEV_ENGINE
{
	enum class BodyType
	{
		Static,
		Dynamic,
		Kinematic,
	};

	class RigidBody
	{
		public:
			RigidBody(BodyType type, const std::shared_ptr<Collider>& collider, float mass, float friction);
			~RigidBody();
			btRigidBody* GetBody();
			void SetAddedToWorld(bool added);
			bool IsAddedToWorld() const;

			BodyType GetBodyType() const;

			void SetBodyPosition(const glm::vec3& pos);
			glm::vec3 GetBodyPosition() const;
			void SetBodyRotation(const glm::quat& rot);
			glm::quat GetBodyRotation()const;
	private:
		std::unique_ptr<btRigidBody> _mRigidBody;
		BodyType _mBodyType = BodyType::Static;
		std::shared_ptr<Collider> _mBodyCollider;
		float _mBodyMass = 0.0f;
		float _mBodyFriction = 0.5f;
		bool _mAddedToWorld = false;
	};
}