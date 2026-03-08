#pragma once
#include <glm/vec3.hpp>

#include "Engine/scene/Component.h"

namespace GAMEDEV_ENGINE
{
	class LightComponent : public Component
	{
	COMPONENT(LightComponent)
	public:
		void Update(float deltaTime) override;
		//----------------------- LoadProperties --------------------
		void LoadProperties(const nlohmann::json& json) override;
		// lets create a setter and getter for the color
		
		// ------------------ Setter --------------------------------
		void SetLightColor(glm::vec3 lightColor);

		// ------------------ Getter --------------------------------
		const glm::vec3 GetLightColor() const { return _mLightColor; }

	private:
		// we need two main properties
			// 1. Position - but position comes from parent GAMEOBJECT. dont Have to store the position seperately
			// 2. Color - we need the color 
		glm::vec3 _mLightColor{ 1.0f };

	};
}