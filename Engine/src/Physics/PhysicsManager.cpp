#include "Engine/Physics/PhysicsManager.h"
#include "Engine/Physics/RigidBody.h"
#include "Engine/Physics/CollisionObject.h"

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

namespace GAMEDEV_ENGINE
{
	PhysicsManager::PhysicsManager()
	{
	}
	PhysicsManager::~PhysicsManager()
	{
	}


	void PhysicsManager::Init()
	{
		// intiate all bullet subsystem in order.
		// then create btDiscreteDynamicWorld
		// then set gravity


		_mBroadPhase = std::make_unique<btDbvtBroadphase>();
		_mCollisionConfig = std::make_unique<btDefaultCollisionConfiguration>();
		_mDispatcher = std::make_unique<btCollisionDispatcher>(_mCollisionConfig.get());
		_mSolver = std::make_unique<btSequentialImpulseConstraintSolver>();
		_mPhysicsWorld = std::make_unique<btDiscreteDynamicsWorld>(
			_mDispatcher.get(),      // 1st: Dispatcher
			_mBroadPhase.get(),      // 2nd: Broadphase  
			_mSolver.get(),          // 3rd: Solver
			_mCollisionConfig.get()  // 4th: Collision config
		);
		_mPhysicsWorld ->setGravity(btVector3(0, -9.81f, 0));
	}

	void PhysicsManager::Update(float deltaTime)
	{
		const btScalar fixedTimeStep = 1.0f / 60.f;
		const int maxSubSteps = 4;
		_mPhysicsWorld->stepSimulation(deltaTime, maxSubSteps, fixedTimeStep);

		// process collisions
		auto dispatcher = _mPhysicsWorld->getDispatcher();
		const auto numManifolds = dispatcher->getNumManifolds();
		for (int i = 0; i < numManifolds; ++i)
		{
			auto manifold = dispatcher->getManifoldByIndexInternal(i);
			if (!manifold)
			{
				continue;
			}

			auto bodyA = reinterpret_cast<CollisionObject*>(manifold->getBody0()->getUserPointer());
			auto bodyB = reinterpret_cast<CollisionObject*>(manifold->getBody1()->getUserPointer());

			if (!bodyA || !bodyB)
			{
				continue;
			}

			const auto numContacts = manifold->getNumContacts();
			for (int j = 0; j < numContacts; ++i)
			{
				const auto& point = manifold->getContactPoint(j);
				const glm::vec3 pos(
					point.m_positionWorldOnB.x(),
					point.m_positionWorldOnB.y(),
					point.m_positionWorldOnB.z());
				const glm::vec3 norm(
					point.m_normalWorldOnB.x(),
					point.m_normalWorldOnB.y(),
					point.m_normalWorldOnB.z());

				bodyA->DispatchContactEvent(bodyB, pos, norm);
				bodyB->DispatchContactEvent(bodyA, pos, norm);
			}
		}

	}

	void PhysicsManager::AddRigidBody(RigidBody* body)
	{
		if (!body || !_mPhysicsWorld) return;
		if (auto rigidBody = body->GetBody())
		{
			if (body->GetBodyType() == BodyType::Static)
			{
				_mPhysicsWorld->addRigidBody(rigidBody,
					btBroadphaseProxy::StaticFilter,
					btBroadphaseProxy::AllFilter);
			}
			else
			{
				// Dynamic and Kinematic use default filters
				_mPhysicsWorld->addRigidBody(rigidBody);
			}
			body->SetAddedToWorld(true);
		}
	}

	void PhysicsManager::RemoveRigidBody(RigidBody* body)
	{
		if (!body || !_mPhysicsWorld)
		{
			return;
		}

		if (auto rigidBody = body->GetBody())
		{
			_mPhysicsWorld->removeRigidBody(rigidBody);
			body->SetAddedToWorld(false);
		}
	}

	btDiscreteDynamicsWorld* PhysicsManager::GetWorld()
	{
		return _mPhysicsWorld.get();
	}
}

