#include <iostream>

#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include "Input/Input.h"

#include "RaytracerApp.h"

int main()
{
	// init RaytracerApp
	RaytracerApp raytracerApp;
	if (!raytracerApp.Init())
	{
		std::cerr << "RaytracerApp init failed!" << std::endl;
		return -1;
	}

	// init glfw
	if (!glfwInit())
	{
		std::cerr << "GLFW init failed!" << std::endl;
		return -1;
	}

	// set window configuration
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // only support modern opengl

	// create the window
	GLFWwindow* window = glfwCreateWindow(raytracerApp.GetWidth(), raytracerApp.GetHeight(), raytracerApp.GetTitle(), nullptr, nullptr);
	if (!window)
	{
		std::cerr << "GLFW failed to create the window!" << std::endl;
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Init glew
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "GLEW init failed!" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	// init input
	Input::SetWindow(window);
	Input::SetListener(&raytracerApp);
	glfwSetKeyCallback(window, Input::OnKeyCallback);
	glfwSetMouseButtonCallback(window, Input::OnMouseButtonCallback);
	glfwSetScrollCallback(window, Input::OnMouseScrollCallback);
	glfwSetCursorPosCallback(window, Input::OnMouseMoveCallback);

	// main loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		raytracerApp.MainLoop();

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}
