#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Engine/Common.h"

struct ma_sound;
struct ma_decoder;

namespace GAMEDEV_ENGINE
{
    struct MaSoundDeleter   { void operator()(ma_sound* p) const; };
    struct MaDecoderDeleter { void operator()(ma_decoder* p) const; };

    class Audio
    {
    public:
        ~Audio();
        void SetPosition(const glm::vec3& position);
        void Play(bool loop = false);
        void Stop();
        bool IsPlaying() const;
        void SetVolume(float volume);
        float GetVolume() const;
        static std::shared_ptr<Audio> Load(const std::string& path);

    private:
        std::unique_ptr<ma_sound,   MaSoundDeleter>   _mSound;
        std::unique_ptr<ma_decoder, MaDecoderDeleter> _mDecoder;
        std::vector<char> _mBuffer;
    };
}
