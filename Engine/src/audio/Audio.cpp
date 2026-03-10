#include "Engine/audio/Audio.h"
#include "Engine/Engine.h"
#include <miniaudio.h>  // no MINIAUDIO_IMPLEMENTATION here

namespace GAMEDEV_ENGINE
{
    // ── Deleter implementations ───────────────────────────────────────────
    void MaSoundDeleter::operator()(ma_sound* p) const
    {
        if (p) { ma_sound_uninit(p); delete p; }
    }

    void MaDecoderDeleter::operator()(ma_decoder* p) const
    {
        if (p) { ma_decoder_uninit(p); delete p; }
    }

    // ── Destructor ────────────────────────────────────────────────────────
    Audio::~Audio()
    {
        _mSound.reset();    // MaSoundDeleter runs
        _mDecoder.reset();  // MaDecoderDeleter runs
    }

    void Audio::SetPosition(const glm::vec3& position)
    {
        if (_mSound)
            ma_sound_set_position(_mSound.get(), position.x, position.y, position.z);
    }

    void Audio::Play(bool loop)
    {
        if (_mSound)
        {
            ma_sound_start(_mSound.get());
            ma_sound_set_looping(_mSound.get(), loop ? MA_TRUE : MA_FALSE);
        }
    }

    void Audio::Stop()
    {
        if (_mSound)
        {
            ma_sound_stop(_mSound.get());
            ma_sound_seek_to_pcm_frame(_mSound.get(), 0);
        }
    }

    bool Audio::IsPlaying() const
    {
        if (_mSound)
            return ma_sound_is_playing(_mSound.get());
        return false;
    }

    void Audio::SetVolume(float volume)
    {
        if (_mSound)
            ma_sound_set_volume(_mSound.get(), std::clamp(volume, 0.0f, 1.0f));
    }

    float Audio::GetVolume() const
    {
        if (_mSound)
            return ma_sound_get_volume(_mSound.get());
        return 0.0f;
    }

    std::shared_ptr<Audio> Audio::Load(const std::string& path)
    {
        auto buffer = Engine::GetInstance().GetAssetFileSystem().LoadAssetFile(path);
        auto engine = Engine::GetInstance().GetAudioManager().GetEngine();

        auto audio = std::make_shared<Audio>();
        audio->_mBuffer = buffer;

        audio->_mDecoder = std::unique_ptr<ma_decoder, MaDecoderDeleter>(new ma_decoder());
        auto result = ma_decoder_init_memory(
            audio->_mBuffer.data(), audio->_mBuffer.size(),
            nullptr, audio->_mDecoder.get()
        );
        if (result != MA_SUCCESS)
            return nullptr;

        audio->_mSound = std::unique_ptr<ma_sound, MaSoundDeleter>(new ma_sound());
        result = ma_sound_init_from_data_source(
            engine, audio->_mDecoder.get(), 0, NULL, audio->_mSound.get()
        );
        if (result != MA_SUCCESS)
            return nullptr;

        ma_sound_set_spatialization_enabled(audio->_mSound.get(), MA_TRUE);
        return audio;
    }
}