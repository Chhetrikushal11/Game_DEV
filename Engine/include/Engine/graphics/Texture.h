#pragma once
#include <GL/glew.h>        // 1. GLEW FIRST!
#include <GLFW/glfw3.h> 
#include  <memory>
#include <string>

namespace GAMEDEV_ENGINE
{
	class Texture
	{
	public:
		// creating constructor with parameters
		Texture(int width, int height, int numChannels, unsigned char* data);

		// add destructor
		~Texture();


		// to get the id we will add simple Getter method
		GLuint GetTextureID() const { return _mTextureID; }

		// need to add the helper method 
		void Init(int width, int height, int numChannels, unsigned char* data);

		// getting texture method
		static std::shared_ptr<Texture> Load(const std::string& path);
	private:
		int _mTextureWidth = 0;
		int _mTextureHeight = 0;
		int _mTextureNumChannels = 0;
		// declare the Gluint
		GLuint _mTextureID = 0;
	};
}