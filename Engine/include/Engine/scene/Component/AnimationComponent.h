#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include "Engine/scene/Component.h"
namespace GAMEDEV_ENGINE
{
	// for position and scale
	struct KeyFrameVec3
	{
		float time = 0.0f;
		glm::vec3 value = glm::vec3(0.0f); // for position and scale
	};
	 
	// for rotation quateranion key frame
	struct KeyFrameQuat
	{
		float time = 0.0f;
		glm::quat value = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	};

	struct TransformTrack
	{
		std::string targetName;
		std::vector<KeyFrameVec3> positions;
		std::vector<KeyFrameQuat> rotations;
		std::vector<KeyFrameVec3> scales;
	};

	// for animation clip
	struct AnimationClip
	{
		std::string name;
		float duration = 0.0f; // for whole duration of clip
		bool looping = true;
		std::vector<TransformTrack> tracks;
	};

	// for tracking the tracks
	struct ObjectBinding
	{
		GameObject* object = nullptr;
		std::vector<size_t> trackIndices; // indices from the AnimationClips
	};

	class AnimationComponent : public Component
	{
		COMPONENT(AnimationComponent)
	public:
		void Update(float deltaTime) override;
		void SetClip(AnimationClip* clip);
		void RegisterClip(const std::string& name, const std::shared_ptr<AnimationClip>& clip);
		void Play(const std::string& name, bool loop);

	private:
		void BuildBindings();
		glm::vec3 InterpolateV3(const std::vector<KeyFrameVec3>& keys, float time);
		glm::quat InterpolateQ4(const std::vector<KeyFrameQuat>& keys, float time);

	private:
		AnimationClip* _mClip = nullptr;
		float _mTime = 0.0f;
		bool _mLooping = true;
		bool _mIsPlaying = false;

		std::unordered_map<std::string, std::shared_ptr<AnimationClip>> _mClips;
		std::unordered_map <GameObject*, std::unique_ptr<ObjectBinding>> _mBindings;
	};
}