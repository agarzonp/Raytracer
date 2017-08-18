#ifndef RAYTRACER_APP_MACHINE_INTERFACE_H
#define RAYTRACER_APP_MACHINE_INTERFACE_H

enum class ERaytracerAppState
{
  CONFIG_RAYTRACER,
  RENDER,
  SELECT_SCENE
};

class RaytracerAppMachineInterface
{
public:

  virtual ~RaytracerAppMachineInterface() {};
  virtual void SetState(ERaytracerAppState state) {};
};

#endif // !RAYTRACER_APP_MACHINE_INTERFACE_H

