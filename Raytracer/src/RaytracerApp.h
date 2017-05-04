#ifndef RAYTRACER_APP_H
#define RAYTRACER_APP_H

#include <string>

#include "Input/Input.h"

class RaytracerApp : public InputListener
{
	// window width and height
	int width;
	int height;

	std::string title;

public:
	RaytracerApp()
		: width(-1)
		, height(-1)
	{};
	~RaytracerApp() {};

	int GetWidth() { return width; }
	int GetHeight() { return height; }

	const char* GetTitle() { return title.c_str(); }

	bool Init()
	{
		// TO-DO: read a configuration file and set the properties
		width = 800;
		height = 600;
		title = "Raytracing";

		return true;
	}

	void MainLoop()
	{
		Update();
		Render();
	}

private:

	void Update()
	{
		// Update proper state via state machine
	}

	void Render()
	{
		// Render proper state via state machine
	}
};

#endif // !RAYTRACER_APP_H
