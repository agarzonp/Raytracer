#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <chrono>
#include <sstream>

#include "../ThreadPool/ThreadPool.h"

#include "../Geom3D/Geom3D.h"

#include "Camera/Camera.h"

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
		
    // camera
    Camera camera(width, height);

		for (int y = height - 1; y >= 0; y--)
		{
			for (int x = 0; x < width; x++)
			{
				// ray generation
				float u = float(x) / float(width);
				float v = float(y) / float(height);

				Geom3D::Ray ray = camera.GetRay(u, v);

				// raycast
				Geom3D::RaycastHit raycastHit;
				glm::vec4 colour;
				if(Raycast(ray, raycastHit))
				{
					// shade
					Shade(raycastHit, colour);
				}
				else
				{
					// set background colour: blend from white to blue
					glm::vec3 rayDirNormalized = glm::normalize(ray.Direction());
					float t = 0.5f*(rayDirNormalized.y + 1.0f);
					colour = (glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)*(1.0f - t)) + (glm::vec4(0.5f, 0.7f, 1.0f, 1.0f)*t);
				}
				
				// set pixel colour
				SetPixelColour(x, y, colour);

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

	// raycast
	inline bool Raycast(const Geom3D::Ray& ray, Geom3D::RaycastHit& raycastHit)
	{
		Geom3D::Sphere sphere(glm::vec3(0.0f, 0.0f, -4.0f), 3.0f);
		bool raycast = sphere.Raycast(ray, raycastHit);
		
		return raycast;
	}

	// shade
	inline void Shade(const Geom3D::RaycastHit& raycastHit, glm::vec4& colour)
	{
		// temporal shading: gradient according to the normal 
		const glm::vec3& normal = raycastHit.normal;
		colour = glm::vec4(normal.x + 1.0f, normal.y + 1.0f, normal.z + 1.0f, 2.0f) * 0.5f;
	}

	// set pixel colour
	inline void SetPixelColour(int x, int y, const glm::vec4& colour)
	{
		unsigned pixelIndex = (y*width + x) * 4;

		buffer[pixelIndex + 0] = colour.r;
		buffer[pixelIndex + 1] = colour.g;
		buffer[pixelIndex + 2] = colour.b;
		buffer[pixelIndex + 3] = colour.a;
	}

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

