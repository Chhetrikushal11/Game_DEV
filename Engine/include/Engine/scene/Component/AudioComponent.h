#pragma once
#include "Engine/scene/Component.h"
#include "Engine/audio/Audio.h"

namespace GAMEDEV_ENGINE
{
	class AudioComponent : public Component
	{
		COMPONENT(AudioComponent)
	public:
		virtual void LoadProperties(const nlohmann::json& json) override;
		virtual void Update(float deltaTime) override;

		void RegisterAudio(const std::string& name, std::shared_ptr<Audio>& clip);
		void Play(const std::string& name, bool loop = false);
		void Stop(const std::string& name);
		bool IsPlaying(const std::string& name);
	
	private:
		std::unordered_map<std::string, std::shared_ptr<Audio>> _mClips;

	};
}

