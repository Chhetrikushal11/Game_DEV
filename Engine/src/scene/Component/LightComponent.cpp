#include "Engine/scene/Component/LightComponent.h"

namespace GAMEDEV_ENGINE
{
	void LightComponent::Update(float deltaTime)
	{
	}

	void LightComponent::SetLightColor(glm::vec3 lightColor)
	{
		_mLightColor = lightColor;
	}

}