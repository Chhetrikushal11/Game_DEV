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
		float _mSensitivity = 1.5f; // how quickly we turn when we turn the mouse
		float _mMoveSpeed = 5.0f; // how fast we move when pressing keys

	};
}