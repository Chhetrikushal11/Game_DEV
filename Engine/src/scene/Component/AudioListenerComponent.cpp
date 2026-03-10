#include "Engine/scene/Component/AudioListenerComponent.h"
#include "Engine/scene/GameObject.h"
#include "Engine/Engine.h"
namespace GAMEDEV_ENGINE
{
	void AudioListenerComponent::Update(float deltaTime)
	{
		auto pos = _mGameObjectOwner->GetWorldPosition();
		Engine::GetInstance().GetAudioManager().SetListenerPosition(pos); // will be only one listener
	}
}