#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>

#include "Engine/Engine.h"
#include "Engine/scene/Component/KinematicControllerComponent.h"


namespace GAMEDEV_ENGINE
{
	KinematicsCharacterController::KinematicsCharacterController(float radius, float height)
		: _mRadius{ radius },
		_mHeight{ height }
	{
		auto world = Engine::GetInstance().GetPhysicsManager().GetWorld();

		// 1. Create shape first
		_mCapsule = new btCapsuleShape(_mRadius, _mHeight);

		// 2. Create ghost BEFORE using it
		_mGhost = std::make_unique<btPairCachingGhostObject>();
		btTransform start;
		start.setIdentity();
		start.setOrigin(btVector3(0.0f, 2.0f, 0.0f));
		_mGhost->setWorldTransform(start);
		_mGhost->setCollisionShape(_mCapsule);
		_mGhost->setCollisionFlags(
			_mGhost->getCollisionFlags() | btCollisionObject::CF_CHARACTER_OBJECT);

		// 3. Assign to MEMBER not local variable
		_mGhostPairCallback = new btGhostPairCallback();
		world->getBroadphase()->getOverlappingPairCache()
			->setInternalGhostPairCallback(_mGhostPairCallback);

		// 4. Create controller
		const btScalar stepHeight = 0.35f;
		_mController = std::make_unique<btKinematicCharacterController>(
			_mGhost.get(),
			static_cast<btConvexShape*>(_mCapsule),
			stepHeight
		);

		_mController->setMaxSlope(btRadians(50.0f));
		_mController->setGravity(btVector3(0.0f, -9.81f, 0.0f));

		// 5. Register with world
		world->addCollisionObject(
			_mGhost.get(),
			btBroadphaseProxy::CharacterFilter,
			btBroadphaseProxy::AllFilter & ~btBroadphaseProxy::SensorTrigger);
		world->addAction(_mController.get());
	}

	KinematicsCharacterController::~KinematicsCharacterController()
	{
		auto world = Engine::GetInstance().GetPhysicsManager().GetWorld();

		if (world)
		{
			// 1. Remove the action (controller) first
			//    — it holds a pointer to _mGhost internally
			if (_mController)
			{
				world->removeAction(_mController.get());
			}

			// 2. Remove the ghost object from the world
			//    — must happen after controller is removed
			if (_mGhost)
			{
				world->removeCollisionObject(_mGhost.get());
			}

			// 3. Remove the ghost pair callback from broadphase
			//    — still part of world cleanup, do it while world exists
			if (_mGhostPairCallback)
			{
				world->getBroadphase()->getOverlappingPairCache()
					->setInternalGhostPairCallback(nullptr);
			}
		}

		// 4. Delete the callback — world no longer references it
		delete _mGhostPairCallback;
		_mGhostPairCallback = nullptr;

		// 5. Delete the shape last
		//    — _mGhost and _mController both held a pointer to it
		//    — unique_ptrs above auto-destruct after this scope
		delete _mCapsule;
		_mCapsule = nullptr;
	}

	glm::vec3 KinematicsCharacterController::GetPosition() const
	{
		const auto& pos = _mGhost->getWorldTransform().getOrigin();

		return glm::vec3(
			pos.x(),
			pos.y() + _mHeight + 0.5f + _mRadius,
			pos.z()
		);
	}

	glm::quat KinematicsCharacterController::GetRotation() const
	{
		const auto& rot = _mGhost->getWorldTransform().getRotation();
		return glm::quat(rot.w(), rot.x(), rot.y(), rot.z());


	}
	void KinematicsCharacterController::Walk(const glm::vec3& direction)
	{
		_mController->setWalkDirection(btVector3(
			btScalar(direction.x), 
			btScalar(direction.y), 
			btScalar(direction.z)));
	}

	void KinematicsCharacterController::Jump(const glm::vec3& direction)
	{
		if (_mController->onGround())
		{
			_mController->jump(btVector3(
				btScalar(direction.x),
				btScalar(direction.y),
				btScalar(direction.z)));
			
		}
	}

	bool KinematicsCharacterController::OnGround() const
	{
		return _mController->onGround();
	}

}