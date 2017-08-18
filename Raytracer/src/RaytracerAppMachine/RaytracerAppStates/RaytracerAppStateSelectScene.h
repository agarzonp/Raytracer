#ifndef RAYTRACER_APP_STATE_SELECT_SCENE_H
#define RAYTRACER_APP_STATE_SELECT_SCENE_H

#include "RaytracerAppState.h"

class RaytracerAppStateSelectScene : public RaytracerAppState
{
public:
	RaytracerAppStateSelectScene(RaytracerAppMachineInterface* stateMachine)
    : RaytracerAppState(stateMachine)
  {
  };

	~RaytracerAppStateSelectScene() {};

  // OnKeyPressed
  void OnKeyPressed(int key) override
  {
    switch (key)
    {
    case GLFW_KEY_ESCAPE:
      stateMachine->SetState(ERaytracerAppState::RENDER);
      break;
    case GLFW_KEY_C:
      break;
    }
  }

};

#endif // !RAYTRACER_APP_STATE_SELECT_SCENE_H
