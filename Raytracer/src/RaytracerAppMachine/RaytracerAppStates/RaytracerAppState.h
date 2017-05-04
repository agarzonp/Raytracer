#ifndef RAYTRACER_APP_STATE
#define RAYTRACER_APP_STATE

class RaytracerAppState
{
public:
	RaytracerAppState() {};
	~RaytracerAppState() {};

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

private:

};

#endif // !RAYTRACAER_APP_STATE

