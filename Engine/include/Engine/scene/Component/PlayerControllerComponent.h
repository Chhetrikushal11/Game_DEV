#pragma once

#include "Engine/scene/Component.h"

namespace GAMEDEV_ENGINE
{
	class PlayerControllerComponent : public Component
	{
		COMPONENT(PlayerControllerComponent)
	public:
		void Update(float deltaTime) override;
	
	private:
		// to capture mouse sensitivity and mouse speed
		float _mSensitivity = 0.1f; // how quickly we turn when we turn the mouse
		float _mMoveSpeed = 1.0f; // how fast we move when pressing keys

	};
}