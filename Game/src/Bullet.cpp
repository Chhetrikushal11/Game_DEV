#include "Bullet.h"

namespace GAMEDEV_ENGINE
{
	void Bullet::Update(float deltaTime)
	{
		// call the game object update
		GameObject::Update(deltaTime);
		_mLifeTime -= deltaTime;
		if (_mLifeTime <= 0.0f)
		{
			MarkForDestroy();
		}
	}
}