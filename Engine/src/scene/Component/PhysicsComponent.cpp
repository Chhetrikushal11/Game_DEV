#include "Engine/scene/Component/PhysicsComponent.h"
#include "Engine/scene/GameObject.h"
#include "Engine/Engine.h"

namespace GAMEDEV_ENGINE
{
	PhysicsComponent::PhysicsComponent(const std::shared_ptr<RigidBody>& body)
		: _mComponentRigidBody{body}
	{
	}

	void PhysicsComponent::LoadProperties(const nlohmann::json& json)
	{
		std::shared_ptr<Collider> collider;

		if (json.contains("collider"))
		{
			const auto& colliderObj = json["collider"];
			std::string type = colliderObj.value("type", "");
			if (type == "box")
			{	
				glm::vec3 extents(
					colliderObj.value("x", 1.0f),
					colliderObj.value("y", 1.0f),
					colliderObj.value("z", 1.0f)
				);
				collider = std::make_shared<BoxCollider>(extents);

			}
			else if (type == "sphere")
			{
				float radius = colliderObj.value("r", 1.0f);
				collider = std::make_shared<SphereCollider>(radius);
			}

			else if (type == "capsule")
			{
				float radius = colliderObj.value("r", 1.0f);
				float height = colliderObj.value("h", 1.0f);
				collider = std::make_shared<CapsuleCollider>(radius, height);
			}

			if (!collider)
			{
				return;
			}

			std::shared_ptr<RigidBody> rigidBody;
			if (json.contains("body"))
			{
				const auto& bodyObj = json["body"];
				float mass = bodyObj.value("mass", 0.0f);
				float friction = bodyObj.value("friction", 0.5f);
				std::string typeStr = bodyObj.value("type", "static");

				BodyType type = BodyType::Static;
				if (typeStr == "dynamic")
				{
					type = BodyType::Dynamic;
				}

				else if ((typeStr == "kinematic"))
				{
					type = BodyType::Kinematic;
				}

				rigidBody = std::make_shared<RigidBody>(type, collider, mass, friction);
			}

			if (rigidBody)
			{
				SetRigidBody(rigidBody);
			}

		}

	}

	void PhysicsComponent::Init()
	{
		if (!_mComponentRigidBody)
		{
			return;
		}

		const auto pos = _mGameObjectOwner->GetWorldPosition1();
		const auto rot = _mGameObjectOwner->GetWorldRotation();

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
			_mGameObjectOwner->SetWorldPosition(_mComponentRigidBody->GetBodyPosition());
			_mGameObjectOwner->SetWorldRotation(_mComponentRigidBody->GetBodyRotation());
		}
	}

	void PhysicsComponent::SetRigidBody(const std::shared_ptr<RigidBody>& body)
	{
		_mComponentRigidBody = body;
	}


}