#include "Engine/graphics/Texture.h"
#include "Engine/Engine.h"

// need to define stb image for its implementation
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace GAMEDEV_ENGINE
{
	Texture::Texture(int width, int height, int numChannels, unsigned char* data)
		:_mTextureWidth{ width },
		_mTextureHeight{ height },
		_mTextureNumChannels{ numChannels }
	{
		Init(width, height, numChannels, data);
	}

	Texture::~Texture()
	{
		if (_mTextureID > 0)
		{
			glDeleteTextures(1, &_mTextureID);
		}
	}

	void GAMEDEV_ENGINE::Texture::Init(int width, int height, int numChannels, unsigned char* data)
	{
		// creating the texture in the gpu memory
		glGenTextures(1, &_mTextureID); // here we are callilng please write the gpu ID for 1 texture in _mTextureId
		// now we will bind using glBindTexture
		glBindTexture(GL_TEXTURE_2D, _mTextureID);
		// to make sure the 4 channels or 3 channels so we need a logic to handles the channel counts
		GLint internalFormat = GL_RGB;
		GLenum format = GL_RGB;

		if (numChannels == 4)
		{
			internalFormat = GL_RGBA;
			format = GL_RGBA;
		}

		// now to load data in GPU

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	std::shared_ptr<Texture> GAMEDEV_ENGINE::Texture::Load(const std::string& path)
	{
		// declare for width, height and numChannels
		int width, height, numChannels;

		// need to build path to asset
		auto& afs = Engine::GetInstance().GetAssetFileSystem();
		auto fullpath = afs.GetAssetsFolder() / path;

		if (!std::filesystem::exists(fullpath))
		{
			return nullptr;
		}

		std::shared_ptr<Texture> result;
		unsigned char* data = stbi_load(fullpath.string().c_str(), &width, &height, &numChannels, 0);

		if (data)
		{
			result = std::make_shared<Texture>(width, height, numChannels, data);
			std::cout << "Image Loaded: " << width << "x" << height << " channels: " << numChannels << std::endl;
			std::cout << "Texture ID: " << result->GetTextureID() << std::endl;  // Add this!
			stbi_image_free(data);

		}

		return result;
	}
	
	std::shared_ptr<Texture> TextureManager::GetorLoadTexture(const std::string& path)
	{
		auto it = _mtexture.find(path);
		if (it != _mtexture.end())
		{
			return it->second;
		}

		auto textureNow = Texture::Load(path);
		_mtexture[path] = textureNow;
		return textureNow;
	}

}