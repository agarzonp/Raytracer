#ifndef RAYTRACER_APP_H
#define RAYTRACER_APP_H

#include <string>

#include "Input/Input.h"
#include "Raytracer/Raytracer.h"
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

	// pixelsBuffer
	float* pixelsBuffer = nullptr;

public:
	RaytracerApp(){};
	~RaytracerApp() 
	{
		delete[] pixelsBuffer;
	};

	int GetWidth() { return width; }
	int GetHeight() { return height; }

	const char* GetTitle() { return title.c_str(); }

	// init
	bool Init()
	{
		// TO-DO: read a configuration file and set the properties
		width = 800;
		height = 600;
		title = "Raytracing";
    int antialiasingSamples = 20;
		int recursionDepth = 5;

		// init pixel buffer
		unsigned numPixels = width * height;
		pixelsBuffer = new float[numPixels * 4]; // 4 -> RGBA

		// init state machine
		state = ERaytracerAppState::RENDER;
		raytracerAppMachine.Init(state);

		// init raitracer
		RaytracerConfiguration raytracerConfig;
		raytracerConfig.width = width;
		raytracerConfig.height = height;
    raytracerConfig.antialiasingSamples = antialiasingSamples;
		raytracerConfig.recursionDepth = recursionDepth;
		raytracerConfig.buffer = pixelsBuffer;
		Raytracer::Get().SetConfiguration(raytracerConfig);

		return true;
	}

	// on key pressed
	void OnKeyPressed(int key) override
	{
		auto& currentState = raytracerAppMachine.GetCurrentState();
		if (currentState)
		{
			currentState->OnKeyPressed(key);
		}
	}

	// mainloop
	void MainLoop()
	{
		raytracerAppMachine.Update();
		raytracerAppMachine.Render();

		// draw current pixels buffer
		glDrawPixels(width, height, GL_RGBA, GL_FLOAT, pixelsBuffer);
	}

private:


};

#endif // !RAYTRACER_APP_H
