#pragma once

#include <memory>



#include "Engine/Common.h"


// declaring the struct
struct ma_engine;
namespace GAMEDEV_ENGINE
{
	class AudioManager
	{
	public:
		AudioManager();
		~AudioManager();

		bool Init();
		ma_engine* GetEngine() { return _mEngine.get(); }

		void SetListenerPosition(const glm::vec3& pos);

	private:
		std::unique_ptr<ma_engine> _mEngine;
	};

}