#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <chrono>
#include <sstream>

typedef std::chrono::time_point<std::chrono::system_clock> TimePoint;

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

	// to keep track of rendering time
	TimePoint renderStart;
	std::string renderTimeStr;

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

		renderStart = std::chrono::system_clock::now();
		
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

					OnRenderingEnded(true);
					
					return;
				}
			}		
		}

		OnRenderingEnded(false);
	}

protected:
	Raytracer() : state(RaytracerState::IDLE) {}
	~Raytracer() {};

private:
	
	// on rendering ended
	void OnRenderingEnded(bool cancelled)
	{
		// output render ended status and time
		renderTimeStr = GetTimeStr(renderStart, std::chrono::system_clock::now());
		printf("Rendering %s!. Render took: %s\n", cancelled ? "CANCELLED" : "DONE", renderTimeStr.c_str());

		// back to idle
		state = RaytracerState::IDLE;
	}

	// GetTimeStr
	std::string GetTimeStr(TimePoint start, TimePoint end)
	{
		size_t total = (size_t)std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

		size_t seconds = total / 1000;
		size_t milliseconds = total - (seconds * 1000);

		std::stringstream ss;
		ss << seconds << "s" << " " << milliseconds << "ms";

		return ss.str();
	}
};

#endif // !RAYTRACER_H

