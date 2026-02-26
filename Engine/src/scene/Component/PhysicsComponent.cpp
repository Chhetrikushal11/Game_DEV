#include "Engine/scene/Component/PhysicsComponent.h"
#include "Engine/scene/GameObject.h"
#include "Engine/Engine.h"

namespace GAMEDEV_ENGINE
{
	PhysicsComponent::PhysicsComponent(const std::shared_ptr<RigidBody>& body)
		: _mComponentRigidBody{body}
	{
	}

	void PhysicsComponent::Init()
	{
		if (!_mComponentRigidBody)
		{
			return;
		}

		const auto pos = _mGameObjectOwner->GetWorldPosition();
		const auto rot = _mGameObjectOwner->GetRotation();

		_mComponentRigidBody->SetBodyPosition(pos);
		_mComponentRigidBody->SetBodyRotation(rot);

		Engine::GetInstance().GetPhysicsManager().AddRigidBody(_mComponentRigidBody.get());

	}

	void PhysicsComponent::Update(float deltaTime)
	{
		if (!_mComponentRigidBody)
		{
			return;
		}

		if (_mComponentRigidBody->GetBodyType() == BodyType::Dynamic)
		{
			_mGameObjectOwner->SetPosition(_mComponentRigidBody->GetBodyPosition());
			_mGameObjectOwner->SetRotation(_mComponentRigidBody->GetBodyRotation());
		}
	}


}