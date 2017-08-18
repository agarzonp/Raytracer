#ifndef RAYTRACER_APP_MACHINE_H
#define RAYTRACER_APP_MACHINE_H

#include <memory>

#include "../Input/Input.h"

#include "RaytracerAppStates.h"

class RaytracerAppMachine
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
	void SetState(ERaytracerAppState state)
	{
		// stop current state before setting the new one
		if (currentState)
		{
			currentState->Stop();
		}

		// set the new state
		switch (state)
		{
		case ERaytracerAppState::SELECT_SCENE:
			currentState.reset(new RaytracerAppStateSelectScene());
			break;
		case ERaytracerAppState::RENDER:
			currentState.reset(new RaytracerAppStateRender());
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

