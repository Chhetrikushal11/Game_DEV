#pragma once

#include "Engine/eng.h"

namespace GAMEDEV_ENGINE
{
	class JumpPlatform : public GameObject, public IContactListener
	{
		GAMEOBJECT(JumpPlatform)
	public:
		void Init() override;
	void OnContact(CollisionObject* obj,
			const glm::vec3& pos,
			const glm::vec3& norm) override;


	};
}