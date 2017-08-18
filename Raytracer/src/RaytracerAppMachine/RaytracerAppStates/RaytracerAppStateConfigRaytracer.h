#ifndef RAYTRACER_APP_STATE_CONFIG_RAYTRACER_H
#define RAYTRACER_APP_STATE_CONFIG_RAYTRACER_H

#include "RaytracerAppState.h"

class RaytracerAppStateConfigRaytracer : public RaytracerAppState
{
public:

  RaytracerAppStateConfigRaytracer(RaytracerAppMachineInterface* stateMachine)
  : RaytracerAppState(stateMachine)
  {
  };

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

private:

};

#endif // !RAYTRACER_APP_STATE_CONFIG_RAYTRACER_H
