#include "Engine/scene/Component/AnimationComponent.h"
#include "Engine/scene/GameObject.h"
#include <iostream>

namespace GAMEDEV_ENGINE
{
	void AnimationComponent::Update(float deltaTime)
	{
		if (!_mClip)
		{
			return;
		}

		if (!_mIsPlaying)
		{
			return;
		}

		_mTime += deltaTime;

		if (_mTime > _mClip ->duration)
		{
			if (_mLooping)
			{
				_mTime = std::fmod(_mTime, _mClip->duration);
			}
			else
			{
				_mTime = 0.0f;
				_mIsPlaying = false;
				return;
			}
		}
		static float timer = 0;
		timer += deltaTime;
		if (timer > 1.0f)
		{
			std::cout << "Playing: " << _mClip->name
				<< " Time: " << _mTime << std::endl;
			timer = 0;
		}

		for (auto& binding : _mBindings)
		{
			auto& obj = binding.first;
			auto& trackIndices = binding.second->trackIndices;
			for (auto i : trackIndices)
			{
				auto& track = _mClip->tracks[i];
				if (!track.positions.empty())
				{
					auto pos = InterpolateV3(track.positions, _mTime);
					obj->SetPosition(pos);
				}

				if (!track.rotations.empty())
				{
					auto rot = InterpolateQ4(track.rotations, _mTime);
					obj->SetRotation(rot);

				}

				if (!track.rotations.empty())
				{
					auto scale = InterpolateV3(track.scales, _mTime);
					obj->SetScale(scale);

				}
			}
		}
	}

	void AnimationComponent::SetClip(AnimationClip* clip)
	{
		_mClip = clip;
		BuildBindings();
	}

	void AnimationComponent::RegisterClip(const std::string& name, const std::shared_ptr<AnimationClip>& clip)
	{
		_mClips[name] = clip;
	}

	void AnimationComponent::Play(const std::string& name, bool loop)
	{
		std::cout << "Play called: " << name << " | Clips available: " << _mClips.size() << std::endl;
		if (_mClip && _mClip->name == name)
		{
			_mTime = 0.0f;
			_mIsPlaying = true;
			_mLooping = loop;
			return;
		}

		else
		{
			auto it = _mClips.find(name);
			if (it != _mClips.end())
			{
				SetClip(it->second.get());
				_mTime = 0.0f;
				_mIsPlaying = true;
				_mLooping = loop;
			}
		}
	}

	void AnimationComponent::BuildBindings()
	{
		_mBindings.clear();
		if (!_mClip)
		{
			return;
		}

		for (size_t i = 0; i < _mClip->tracks.size(); ++i)
		{
			auto& track = _mClip->tracks[i];
			auto targetObject = _mGameObjectOwner->FindChildByName(track.targetName);

			if (targetObject)
			{
				auto it = _mBindings.find(targetObject);
				if (it != _mBindings.end())
				{
					auto it = _mBindings.find(targetObject);
					if (it != _mBindings.end())
					{
						it->second->trackIndices.push_back(i);
					}
					else
					{
						auto binding = std::make_unique<ObjectBinding>();
						binding->object = targetObject;
						binding->trackIndices.push_back(i);
						_mBindings.emplace(targetObject, std::move(binding));
					}
				}
			}
		}
	}

	glm::vec3 AnimationComponent::InterpolateV3(const std::vector<KeyFrameVec3>& keys, float time)
	{
		if (keys.empty())
		{
			return glm::vec3(0.0f);
		}

		if (keys.size() == 1)
		{
			return keys[0].value;
		}

		if (time <= keys.front().time)
		{
			return keys.front().value;
		}

		if (time <= keys.back().time)
		{
			return keys.back().value;
		}

		size_t i0 = 0;
		size_t i1 = 0;

		for (size_t i = 1; i < keys.size(); ++i)
		{
			if (time <= keys[i].time)
			{
				i1 = i;
				break;
			}
		}

		i0 = i1 > 0 ? i1 - 1 : 0;

		if (time > keys[i0].time && time <= keys[i1].time)
		{
			float deltaTime = keys[i1].time - keys[i0].time;
			float k = (time - keys[i0].time) / deltaTime;

			return glm::mix(keys[i0].value, keys[i1].value, k);
		}

		return keys.back().value;
	}

	glm::quat AnimationComponent::InterpolateQ4(const std::vector<KeyFrameQuat>& keys, float time)
	{
		if (keys.empty())
		{
			return glm::quat(1, 0, 0, 0); ;
		}

		if (keys.size() == 1)
		{
			return keys[0].value;
		}

		if (time <= keys.front().time)
		{
			return keys.front().value;
		}

		if (time <= keys.back().time)
		{
			return keys.back().value;
		}

		size_t i0 = 0;
		size_t i1 = 0;

		for (size_t i = 1; i < keys.size(); ++i)
		{
			if (time <= keys[i].time)
			{
				i1 = i;
				break;
			}
		}
		i0 = i1 > 0 ? i1 - 1 : 0;
		if (time > keys[i0].time && time <= keys[i1].time)
		{
			float deltaTime = keys[i1].time - keys[i0].time;
			float k = (time - keys[i0].time) / deltaTime;

			return glm::slerp(keys[i0].value, keys[i1].value, k);
		}

		return keys.back().value;
	}




}