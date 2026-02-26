#pragma once
#include <memory>
class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

namespace GAMEDEV_ENGINE
{
	class RigidBody;

	class PhysicsManager
	{
	public:
		PhysicsManager();
		~PhysicsManager();

		void Init();
		void Update(float deltaTime);
		
		void AddRigidBody(RigidBody* body);
		void RemoveRigidBody(RigidBody* body);

		btDiscreteDynamicsWorld* GetWorld();
	private:
		std::unique_ptr<btBroadphaseInterface> _mBroadPhase;
		std::unique_ptr<btDefaultCollisionConfiguration> _mCollisionConfig;
		std::unique_ptr<btCollisionDispatcher> _mDispatcher;
		std::unique_ptr<btSequentialImpulseConstraintSolver> _mSolver;
		std::unique_ptr<btDiscreteDynamicsWorld> _mPhysicsWorld;
	};
}