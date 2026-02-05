#pragma once
#include <GL/glew.h>        // 1. GLEW FIRST!
#include <GLFW/glfw3.h> 

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
	private:
		int _mTextureWidth = 0;
		int _mTextureHeight = 0;
		int _mTextureNumChannels = 0;
		// declare the Gluint
		GLuint _mTextureID = 0;
	};
}