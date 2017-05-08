#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <chrono>
#include <sstream>

#include "../ThreadPool/ThreadPool.h"

#include "../Geom3D/Geom3D.h"

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
	// width and height
	int width = -1;
	int height = -1;

	// pixels buffer
	float* buffer = nullptr;

	// raytracer state
	RaytracerState state = RaytracerState::IDLE;

	// to keep track of rendering time
	TimePoint renderStart;
	std::string renderTimeStr;

	// threadpool
	ThreadPool threadPool;

	// Ray
	Geom3D::Ray ray;
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

			// submit render task to the threadPool so we do the rendering in a different thread
			auto task = std::bind(&Raytracer::Render, this);
			threadPool.AddTask(task);
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
		
		// near plane
		float nearPlaneWidth  = float(width)/100.0f;
		float nearPlaneHeight = float(height)/100.0f;
		glm::vec3 nearPlaneBottomLeft = glm::vec3(-nearPlaneWidth*0.5f, -nearPlaneHeight*0.5f, -1.0f);

		// camera position
		glm::vec3 cameraPos(0.0f, 0.0f, 0.0f);
		
		for (int y = height - 1; y >= 0; y--)
		{
			for (int x = 0; x < width; x++)
			{
				// ray generation
				float u = float(x) / float(width);
				float v = float(y) / float(height);

				glm::vec3 rayDir = nearPlaneBottomLeft + glm::vec3(u * nearPlaneWidth, 0.0f, 0.0f) + glm::vec3(0.0f, v*nearPlaneHeight, 0.0f);
				Geom3D::Ray ray(cameraPos, rayDir);

				// ray intersection
				Geom3D::Sphere sphere(glm::vec3(0.0f, 0.0f, -5.0f), 3.0f);
				bool raycast = sphere.Intersect(ray);

				// shading
				// Temporal blend from white to blue
				glm::vec3 rayDirNormalized = glm::normalize(ray.Direction());
				float t = 0.5f*(rayDirNormalized.y + 1.0f);
				glm::vec4 colour = raycast ? glm::vec4(1.0f, 0.0f, 0.0f, 0.0f) : (glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)*(1.0f - t)) + (glm::vec4(0.5f, 0.7f, 1.0f, 1.0f)*t);
				
				// set pixel colour in the buffer
				unsigned pixelIndex = (y*width + x) * 4;

				buffer[pixelIndex + 0] = colour.r;
				buffer[pixelIndex + 1] = colour.g;
				buffer[pixelIndex + 2] = colour.b;
				buffer[pixelIndex + 3] = colour.a;

				// check for rendering cancelled
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

