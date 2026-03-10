#include "Engine/scene/Component/AudioComponent.h"
#include "Engine/scene/GameObject.h"


namespace GAMEDEV_ENGINE
{
	void AudioComponent::LoadProperties(const nlohmann::json& json)
	{
		// contain array a clip
		if (json.contains("audio"))
		{
			auto& clipsObject = json["audio"];
			for (auto& clip : clipsObject)
			{
				std::string name = clip.value("name", "noname");
				std::string path = clip.value("path","");
				auto audio = Audio::Load(path);
				if (audio)
				{
					float volume = clip.value("volume",1.0f);
					audio->SetVolume(volume);
					RegisterAudio(name, audio);
				}
			}
		}
	}
	void AudioComponent::Update(float deltaTime)
	{
		auto pos = _mGameObjectOwner->GetWorldPosition();
		for (auto& clip : _mClips)
		{
			if (clip.second->IsPlaying())
			{
				clip.second->SetPosition(pos);
			}
		}
	}
	void AudioComponent::RegisterAudio(const std::string& name, std::shared_ptr<Audio>& clip)
	{
		_mClips[name] = clip;
	}
	void AudioComponent::Play(const std::string& name, bool loop)
	{
		auto it = _mClips.find(name);
		if (it != _mClips.end())
		{
			if (it->second)
			{
				it->second->Play(loop);
			}
		}
	}
	void AudioComponent::Stop(const std::string& name)
	{
		auto it = _mClips.find(name);
		if (it != _mClips.end())
		{
			if (it->second)
			{
				it->second->Stop();
			}
		}
	}
	bool AudioComponent::IsPlaying(const std::string& name)
	{
		auto it = _mClips.find(name);
		if (it != _mClips.end())
		{
			if (it->second)
			{
				return	it->second->IsPlaying();
			}
		}

		return false;
	}
	
}