#ifndef RAYTRACER_APP_MACHINE_H
#define RAYTRACER_APP_MACHINE_H

#include "RaytracerAppMachineInterface.h"
#include "RaytracerAppStates.h"


class RaytracerAppMachine : public RaytracerAppMachineInterface
{
	std::unique_ptr<RaytracerAppState> currentState;

public:

  // singleton instance
  static RaytracerAppMachine& Get()
  {
    static RaytracerAppMachine instance;
    return instance;
  }

	// set state
	void SetState(ERaytracerAppState state) override
	{
		// stop current state before setting the new one
		if (currentState)
		{
			currentState->Stop();
		}

		// set the new state
		switch (state)
		{
    case ERaytracerAppState::CONFIG_RAYTRACER:
      currentState.reset(new RaytracerAppStateConfigRaytracer(this));
      break;
		case ERaytracerAppState::SELECT_SCENE:
			currentState.reset(new RaytracerAppStateSelectScene(this));
			break;
		case ERaytracerAppState::RENDER:
			currentState.reset(new RaytracerAppStateRender(this));
			break;
		}

		// init current state
		currentState->Init();
	}

  // on key pressed
  void OnKeyPressed(int key)
  {
    if (currentState)
    {
      currentState->OnKeyPressed(key);
    }
  }

	// update
	void Update()
	{
		currentState->Update();
	}

	// render
	void Render()
	{
		currentState->Render();
	}

protected:

  RaytracerAppMachine() {};


};

#endif // !RAYTRACER_APP_MACHINE_H

