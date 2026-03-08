#include "Engine/scene/Component/LightComponent.h"

namespace GAMEDEV_ENGINE
{
	void LightComponent::Update(float deltaTime)
	{
	}

	void LightComponent::LoadProperties(const nlohmann::json& json)
	{
		if (json.contains("color"))
		{
			const auto& colorObj = json["color"];
			glm::vec3 color(
				colorObj.value("r", 1.0f),
				colorObj.value("g", 1.0f),
				colorObj.value("b", 1.0f)
			);
			SetLightColor(color);
		}
	}

	void LightComponent::SetLightColor(glm::vec3 lightColor)
	{
		_mLightColor = lightColor;
	}

}