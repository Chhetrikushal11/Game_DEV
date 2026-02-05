#include "Engine/graphics/Texture.h"

namespace GAMEDEV_ENGINE
{
	Texture::Texture(int width, int height, int numChannels, unsigned char* data)
		:_mTextureWidth{ width },
		_mTextureHeight{ height },
		_mTextureNumChannels{ numChannels }
	{
		// creating the texture in the gpu memory
		glGenTextures(1, &_mTextureID); // here we are callilng please write the gpu ID for 1 texture in _mTextureId
		// now we will bind using glBindTexture
		glBindTexture(GL_TEXTURE_2D, _mTextureID);

		// now to load data in GPU

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	Texture::~Texture()
	{
		if (_mTextureID > 0)
		{
			glDeleteTextures(1, &_mTextureID);
		}
	}

}