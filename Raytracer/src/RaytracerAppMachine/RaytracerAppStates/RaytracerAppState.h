#ifndef RAYTRACER_APP_STATE
#define RAYTRACER_APP_STATE

#include "../RaytracerAppMachineInterface.h"

class RaytracerAppState
{
protected:

  RaytracerAppMachineInterface* stateMachine = nullptr;

public:
	RaytracerAppState(RaytracerAppMachineInterface* stateMachineInterface) 
    : stateMachine(stateMachineInterface)
  {
  };

	virtual ~RaytracerAppState() {};

	virtual void OnKeyPressed(int key) {};
	virtual void OnKeyReleased(int key) {};
	virtual void OnMouseButtonPressed(int button, double x, double y) {};
	virtual void OnMouseButtonReleased(int button, double x, double y) {};
	virtual void OnMouseScroll(double xOffset, double yOffset) {};
	virtual void OnMouseMove(double x, double y) {};

	virtual void Init() {};
	virtual void Stop() {};

	virtual void Update() {};
	virtual void Render() {};

};

#endif // !RAYTRACAER_APP_STATE

