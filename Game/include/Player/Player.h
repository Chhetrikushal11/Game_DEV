#pragma once

#include <Engine/eng.h>

namespace GAMEDEV_ENGINE
{
	class Player : public GameObject
	{
		GAMEOBJECT(Player)
	public:
		 virtual void Init();
		void Update(float deltaTime) override;

	private:
		AnimationComponent* _mAnimationComponent = nullptr;
	};
}