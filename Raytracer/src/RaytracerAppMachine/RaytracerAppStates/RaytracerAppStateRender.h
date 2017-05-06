#ifndef RAYTRACER_APP_STATE_RENDER
#define RAYTRACER_APP_STATE_RENDER

#include "RaytracerAppState.h"

class RaytracerAppStateRender : public RaytracerAppState
{

public:
	RaytracerAppStateRender() {};
	~RaytracerAppStateRender() {};

	// OnKeyPressed
	void OnKeyPressed(int key) override
	{
		switch (key)
		{
		case GLFW_KEY_S:
			Raytracer::Get().StartRendering();
			break;
		case GLFW_KEY_C:
			Raytracer::Get().CancelRendering();
			break;
		}
	}

	void Render() override
	{
		// TO-DO: visualize rendering progress
	}
private:

};

#endif // !RAYTRACER_APP_STATE_RENDER

