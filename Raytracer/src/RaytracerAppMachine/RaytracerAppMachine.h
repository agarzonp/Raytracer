#ifndef RAYTRACER_APP_MACHINE_H
#define RAYTRACER_APP_MACHINE_H

#include <memory>

#include "RaytracerAppStates.h"

class RaytracerAppMachine
{
	std::unique_ptr<RaytracerAppState> currentState;

public:
	RaytracerAppMachine() {};
	~RaytracerAppMachine() {};

	void Init(ERaytracerAppState state)
	{
		SetState(state);
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

private:

};

#endif // !RAYTRACER_APP_MACHINE_H

