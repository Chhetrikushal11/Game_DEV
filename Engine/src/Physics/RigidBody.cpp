#include "Engine/Physics/RigidBody.h"
#include "Engine/Engine.h"
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

namespace GAMEDEV_ENGINE
{
	RigidBody::RigidBody(BodyType type, const std::shared_ptr<Collider>& collider, float mass, float friction)
		: _mBodyType{type},
		_mBodyCollider{collider},
		_mBodyMass{mass},
		_mBodyFriction{friction}
	{
		if (!_mBodyCollider)
		{
			return;
		}

		btVector3 inertia(0, 0, 0);
		if (_mBodyType == BodyType::Dynamic && mass > 0.0f && _mBodyCollider->GetShape())
		{
			_mBodyCollider->GetShape()->calculateLocalInertia(btScalar(mass), inertia);
		}

		btTransform transform;
		transform.setIdentity();
		btDefaultMotionState* motionState = new btDefaultMotionState(transform);
		
		btRigidBody::btRigidBodyConstructionInfo info(
			(_mBodyType == BodyType::Dynamic) ? btScalar(mass) : btScalar(0.0),
			motionState,
			_mBodyCollider->GetShape(),
			inertia
		);

		_mRigidBody = std::make_unique<btRigidBody>(info);
		_mRigidBody->setFriction(friction);

		if (_mBodyType == BodyType::Kinematic)
		{
			_mRigidBody->setCollisionFlags(_mRigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
			_mRigidBody->setActivationState(DISABLE_DEACTIVATION);

		}

	}
	RigidBody::~RigidBody()
	{
		if (_mAddedToWorld)
		{
			Engine::GetInstance().GetPhysicsManager().RemoveRigidBody(this);
		}
	}
	btRigidBody* RigidBody::GetBody()
	{
		return _mRigidBody.get();
	}
	void RigidBody::SetAddedToWorld(bool added)
	{
		_mAddedToWorld = added;
	}
	bool RigidBody::IsAddedToWorld() const
	{
		return _mAddedToWorld;
	}
	BodyType RigidBody::GetBodyType() const
	{
		return _mBodyType;
	}
	void RigidBody::SetBodyPosition(const glm::vec3& pos)
	{
		if (!_mRigidBody)
		{
			return;
		}
		auto& tr = _mRigidBody->getWorldTransform();
		tr.setOrigin(btVector3(btScalar(pos.x), btScalar(pos.y), btScalar(pos.z)));
		if (_mRigidBody->getMotionState())
		{
			_mRigidBody->getMotionState()->setWorldTransform(tr);
		}
		_mRigidBody->setWorldTransform(tr);
	}

	glm::vec3 RigidBody::GetBodyPosition() const
	{
		const auto& pos = _mRigidBody->getWorldTransform().getOrigin();
		return glm::vec3(pos.x(), pos.y(), pos.z());
	}
	void RigidBody::SetBodyRotation(const glm::quat& rot)
	{
		if (!_mRigidBody)
		{
			return;
		}
		auto& tr = _mRigidBody->getWorldTransform();
		tr.setRotation(btQuaternion(btScalar(rot.x), btScalar(rot.y), btScalar(rot.z), btScalar(rot.w)));
		if (_mRigidBody->getMotionState())
		{
			_mRigidBody->getMotionState()->setWorldTransform(tr);
		}
		_mRigidBody->setWorldTransform(tr);
	}
	glm::quat RigidBody::GetBodyRotation() const
	{
		const auto& rot = _mRigidBody->getWorldTransform().getRotation();
		return glm::quat(rot.w(),rot.x(), rot.y(), rot.z());
	}
}