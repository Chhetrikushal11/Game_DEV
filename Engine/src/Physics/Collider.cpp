#include "Engine/Physics/Collider.h"

#include <btBulletCollisionCommon.h>

namespace GAMEDEV_ENGINE
{
	Collider::~Collider()
	{
		if (_mShape)
		{
			delete _mShape;
		}
	}

	btCollisionShape* Collider::GetShape()
	{
		return _mShape;
	}
	BoxCollider::BoxCollider(const glm::vec3& extents)
	{
		glm::vec3 halfExtents = extents * 0.5f;
		_mShape = new btBoxShape(btVector3(halfExtents.x, halfExtents.y, halfExtents.z));
	}

	SphereCollider::SphereCollider(float radius)
	{
		_mShape = new btSphereShape(radius);
	}

	CapsuleCollider::CapsuleCollider(float radius, float height)
	{
		_mShape = new btCapsuleShape(btScalar(radius), btScalar(height));
	}
}