#pragma once

#include <memory>

#include "Engine/scene/Component.h"
#include"Engine/scene/Component/KinematicControllerComponent.h"

namespace GAMEDEV_ENGINE
{
	class PlayerControllerComponent : public Component
	{
		COMPONENT(PlayerControllerComponent)
	public:
		void Init() override;
		void Update(float deltaTime) override;
	
	private:
		// to capture mouse sensitivity and mouse speed
		float _mSensitivity = 1.0f; // how quickly we turn when we turn the mouse
		float _mMoveSpeed = 2.0f; // how fast we move when pressing keys
		float _mXRot = 0.0f;
		float _mYRot = 0.0f;
		bool _mCursorLocked = true;
		bool _mTabWasPressed = false;
		std::unique_ptr<KinematicsCharacterController> _mKinematicsController;
	};
}