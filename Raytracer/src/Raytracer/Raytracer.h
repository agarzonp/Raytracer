#ifndef RAYTRACER_H
#define RAYTRACER_H

enum class RaytracerState
{
	IDLE,
	RENDERING,
	RENDERING_CANCELLED
};

struct RaytracerConfiguration
{
	int width = -1;
	int height = -1;
	float* buffer = nullptr;
};

class Raytracer
{
	int width = -1;
	int height = -1;
	float* buffer = nullptr;

	RaytracerState state = RaytracerState::IDLE;

public:
	
	// Singleton instance
	static Raytracer& Get()
	{
		static Raytracer instance;
		return instance;
	}

	// set configuration
	void SetConfiguration(const RaytracerConfiguration& config)
	{
		width = config.width;
		height = config.height;
		buffer = config.buffer;
	}


	// start rendering
	void StartRendering()
	{
		if (state == RaytracerState::IDLE)
		{
			state = RaytracerState::RENDERING;

			// TO-DO: launch Render function in a new thread!	
			Render();
		}
	}

	// cancel rendering
	void CancelRendering()
	{
		if (state == RaytracerState::RENDERING)
		{
			state = RaytracerState::RENDERING_CANCELLED;
		}
	}

	// render
	void Render()
	{
		printf("Rendering STARTED!\n");
		printf("Rendering...\n");

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				// ray generation

				// ray intersection

				// shading

				if (state == RaytracerState::RENDERING_CANCELLED)
				{
					// early exit if the rendering has been cancelled
					// Warning: race condition allowed, at worst another iteration

					// back to idle
					state = RaytracerState::IDLE;

					printf("Rendering CANCELLED!\n");
					return;
				}
			}		
		}
		
		printf("Rendering DONE!\n");

		// back to idle
		state = RaytracerState::IDLE;
	}

protected:
	Raytracer() : state(RaytracerState::IDLE) {}
	~Raytracer() {};

private:
	
};

#endif // !RAYTRACER_H

