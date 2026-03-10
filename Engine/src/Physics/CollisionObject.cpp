#include "Engine/Physics/CollisionObject.h"

namespace GAMEDEV_ENGINE
{
	CollisionObjectType CollisionObject::GetCollisionObjectType()
	{
		return _mType;
	}
	void CollisionObject::AddContactListener(IContactListener* listener)
	{
		_mContactListeners.push_back(listener);
	}
	void CollisionObject::RemoveContactListener(IContactListener* listener)
	{
		auto it = std::find(_mContactListeners.begin(), _mContactListeners.end(), listener);
		if (it != _mContactListeners.end())
		{
			_mContactListeners.erase(it);
		}
	}
	void CollisionObject::DispatchContactEvent(CollisionObject* obj, const glm::vec3& pos, const glm::vec3& norm)
	{
		for (auto listener : _mContactListeners)
		{
			listener->OnContact(obj, pos, norm);
		}
	}
}