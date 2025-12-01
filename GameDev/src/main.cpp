#include <iostream>
#include <GLFW/glfw3.h>



int main()
{
	if (!glfwInit()) return -1;
	GLFWwindow* window = glfwCreateWindow(1280, 720, "TITLE WINDOW", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Error creating window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// to define window position
	glfwSetWindowPos(window, 2000, 150);
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}

	glfwTerminate();
	
	return 0;
}