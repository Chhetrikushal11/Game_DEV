#include "Engine/Physics/PhysicsManager.h"
#include "Engine/Physics/RigidBody.h"

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
	}
	void PhysicsManager::AddRigidBody(RigidBody* body)
	{
		if (!body || !_mPhysicsWorld)
		{
			return;
		}

		if (auto rigidBody = body->GetBody())
		{
			_mPhysicsWorld->addRigidBody(rigidBody, btBroadphaseProxy::StaticFilter, btBroadphaseProxy::AllFilter);
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

