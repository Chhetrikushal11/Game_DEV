#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>
#include <btBulletDynamicsCommon.h>

class btPairCachingGhostObject;
class btKinematicCharacterController;
class btGhostPairCallback;

namespace GAMEDEV_ENGINE
{
	class KinematicsCharacterController
	{
	public:
		KinematicsCharacterController(float radius, float height);
		~KinematicsCharacterController();

		glm::vec3 GetPosition() const;
		glm::quat GetRotation() const;

		void Walk(const glm::vec3& direction);
		void Jump(const glm::vec3& direction);
		bool OnGround() const;

	private:
		float _mHeight = 1.2f;
		float _mRadius = 0.4f;
		btCapsuleShape* _mCapsule = nullptr;
		btGhostPairCallback* _mGhostPairCallback = nullptr;
		std::unique_ptr<btPairCachingGhostObject> _mGhost;
		std::unique_ptr<btKinematicCharacterController> _mController;
	};

}