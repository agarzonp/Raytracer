#ifndef RAYTRACER_APP_H
#define RAYTRACER_APP_H

#include <string>

#include "CSVParser/CSVParser.h"
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
		// read raytracer configuration from a file the properties
		agarzonp::CSVParser parser("config/config1.csv");
		if (!parser.IsValid())
		{
			return false;
		}

		width = std::stoi(parser[0][1]);
		height = std::stoi(parser[1][1]);
		title = "Raytracing";
    int antialiasingSamples = std::stoi(parser[2][1]);
		int recursionDepth = std::stoi(parser[3][1]);
		int numWorkingThreads = std::stoi(parser[4][1]);
		bool useBVH = std::stoi(parser[5][1]) > 0;
		int randomShapes = std::stoi(parser[6][1]);

		// init pixel buffer
		unsigned numPixels = width * height;
		pixelsBuffer = new float[numPixels * 4]; // 4 -> RGBA

		// init state machine
		state = ERaytracerAppState::RENDER;
		raytracerAppMachine.Init(state);

		// init raytracer
		RaytracerConfiguration raytracerConfig;
		raytracerConfig.width = width;
		raytracerConfig.height = height;
    raytracerConfig.antialiasingSamples = antialiasingSamples;
		raytracerConfig.recursionDepth = recursionDepth;
		raytracerConfig.numWorkingthreads = numWorkingThreads;
		raytracerConfig.useBVH = useBVH;
		raytracerConfig.randomShapes = randomShapes;

		raytracerConfig.buffer = pixelsBuffer;
		Raytracer::Get().Init(raytracerConfig);

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
