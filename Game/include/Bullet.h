#pragma once

#include "Engine/eng.h"

namespace GAMEDEV_ENGINE
{
	class Bullet : public GameObject
	{
		GAMEOBJECT(Bullet)
	public:
		void Update(float deltaTime) override;

	private:
		float _mLifeTime = 2.0f;


	};
}


