#pragma once
#include "Engine/scene/Component.h"


namespace GAMEDEV_ENGINE
{
	class AudioListenerComponent : public Component
	{
		COMPONENT(AudioListenerComponent)
	public:
		void Update(float deltaTime) override;

	};
}