#ifndef RAYTRACER_APP_H
#define RAYTRACER_APP_H

#include <string>

#include "Input/Input.h"
#include "RaytracerAppMachine/RaytracerAppMachine.h"

class RaytracerApp : public InputListener
{
	// window width and height
	int width = -1;
	int height = -1;

	// window title
	std::string title;

	// current state and the state machine
	ERaytracerAppState state;
	RaytracerAppMachine raytracerAppMachine;

public:
	RaytracerApp(){};
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

		// init state machine
		state = ERaytracerAppState::RENDER;
		raytracerAppMachine.Init(state);

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
		raytracerAppMachine.Update();
	}

	void Render()
	{
		raytracerAppMachine.Render();
	}
};

#endif // !RAYTRACER_APP_H
