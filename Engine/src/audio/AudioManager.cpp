#include "Engine/audio/AudioManager.h"
#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

namespace GAMEDEV_ENGINE
{
    AudioManager::AudioManager()
    {
        _mEngine = std::make_unique<ma_engine>();
    }

    AudioManager::~AudioManager()
    {
        if (_mEngine)
        {
            ma_engine_uninit(_mEngine.get());
        }
    }

    bool AudioManager::Init()
    {
        auto result = ma_engine_init(nullptr, _mEngine.get());
        return result == MA_SUCCESS;
    }

    void AudioManager::SetListenerPosition(const glm::vec3& pos)
    {
        if (_mEngine)
        {
            ma_engine_listener_set_position(_mEngine.get(), 0, pos.x, pos.y, pos.z);
        }
    }
}
