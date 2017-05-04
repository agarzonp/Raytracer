#ifndef RAYTRACER_APP_STATES_H
#define RAYTRACER_APP_STATES_H

enum class ERaytracerAppState
{
	SELECT_SCENE,
	RENDER
};

#include "RaytracerAppStates/RaytracerAppState.h"
#include "RaytracerAppStates/RaytracerAppStateSelectScene.h"
#include "RaytracerAppStates/RaytracerAppStateRender.h"

#endif // !RAYTRACER_APP_STATES_H



