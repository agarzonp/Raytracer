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
    // read app configuration from a file
    agarzonp::CSVParser appConfig("config/appConfig.csv");
    if (!appConfig.IsValid())
    {
      return false;
    }

    width = std::stoi(appConfig[0][1]);
    height = std::stoi(appConfig[1][1]);
    title = appConfig[2][1];

    // init pixel buffer
    unsigned numPixels = width * height;
    pixelsBuffer = new float[numPixels * 4]; // 4 -> RGBA

		// read raytracer configuration from a file
		agarzonp::CSVParser parser("config/raytracer/config1.csv");
		if (!parser.IsValid())
		{
			return false;
		}

    int antialiasingSamples = std::stoi(parser[0][1]);
		int recursionDepth = std::stoi(parser[1][1]);
		int numWorkingThreads = std::stoi(parser[2][1]);
		bool useBVH = std::stoi(parser[3][1]) > 0;
		int randomShapes = std::stoi(parser[4][1]);

		// init raytracer
		RaytracerConfiguration raytracerConfig;
		raytracerConfig.width = width;
		raytracerConfig.height = height;
    raytracerConfig.buffer = pixelsBuffer;
    raytracerConfig.antialiasingSamplesCount = antialiasingSamples;
		raytracerConfig.maxRecursionDepth = recursionDepth;
		raytracerConfig.renderingSubtasksCount = numWorkingThreads;
		raytracerConfig.useBVH = useBVH;
		raytracerConfig.randomShapes = randomShapes;
		
		Raytracer::Get().Init(raytracerConfig);

    // init state machine
    RaytracerAppMachine::Get().SetState(ERaytracerAppState::RENDER);
		return true;
	}

	// on key pressed
	void OnKeyPressed(int key) override
	{
    RaytracerAppMachine::Get().OnKeyPressed(key);
	}

	// mainloop
	void MainLoop()
	{
    RaytracerAppMachine::Get().Update();
    RaytracerAppMachine::Get().Render();

		// draw current pixels buffer
		glDrawPixels(width, height, GL_RGBA, GL_FLOAT, pixelsBuffer);
	}

private:


};

#endif // !RAYTRACER_APP_H
