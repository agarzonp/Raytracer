#ifndef RAYTRACER_APP_STATE_RENDER
#define RAYTRACER_APP_STATE_RENDER

#include "RaytracerAppState.h"

class RaytracerAppStateRender : public RaytracerAppState
{

public:

  RaytracerAppStateRender(RaytracerAppMachineInterface* stateMachine)
    : RaytracerAppState(stateMachine)
  {
  };

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
    case GLFW_KEY_1:
      if (!Raytracer::Get().IsRendering())
      {
        stateMachine->SetState(ERaytracerAppState::CONFIG_RAYTRACER);
      }
      break;
    case GLFW_KEY_2:
      if (!Raytracer::Get().IsRendering())
      {
        stateMachine->SetState(ERaytracerAppState::SELECT_SCENE);
      }
      break;

    }
	}

};

#endif // !RAYTRACER_APP_STATE_RENDER

