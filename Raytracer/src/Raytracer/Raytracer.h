#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <chrono>
#include <random>
#include <sstream>

#include "../ThreadPool/ThreadPool.h"

#include "../Geom3D/Geom3D.h"

#include "Camera/Camera.h"
#include "Materials/Materials.h"

typedef std::chrono::time_point<std::chrono::system_clock> TimePoint;

// random number generation
std::random_device randomDevice;
std::mt19937 randomEngine(randomDevice());
std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

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
  int antialiasingSamples = 1;
	int recursionDepth = 1;
	float* buffer = nullptr;
};

class Raytracer
{
	// width and height
	int width = -1;
	int height = -1;

  // antialiasing samples
  int antialiasingSamples = 1;

	// max recursion
	int maxRecursion = 1;

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
    antialiasingSamples = config.antialiasingSamples;
		maxRecursion = config.recursionDepth;
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
        //calculate pixel colour
        glm::vec4 pixelColour = CalculatePixelColour(x, y, camera);
     
				// set pixel colour
				SetPixelColour(x, y, pixelColour);

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

  // calculate pixel colour
  inline glm::vec4 CalculatePixelColour(int x, int y, Camera& camera)
  {
    // Note: we send more than one ray per pixel (randomly offset) in order to do antialiasing

    glm::vec3 pixelColour(0.0f, 0.0f, 0.0f);
    for (int sample = 0; sample < antialiasingSamples; sample++)
    {
      // ray generation
      float xOffset = distribution(randomEngine);
      float yOffset = distribution(randomEngine);
      float u = (float(x) + xOffset) / float(width);
      float v = (float(y) + yOffset) / float(height);

      Geom3D::Ray ray = camera.GetRay(u, v);

			// calculate pixel colour for the following ray
			pixelColour += CalculatePixelColour(ray, 0);
		}

    // avarage the colour
    pixelColour /= float(antialiasingSamples);

    return glm::vec4(pixelColour, 1.0f);
  }

	// calculate pixel colour
	glm::vec3 CalculatePixelColour(const Geom3D::Ray& ray, int recursionDepth)
	{
		// raycast
		Geom3D::RaycastHit raycastHit;
		if (recursionDepth < maxRecursion && Raycast(ray, recursionDepth > 0 ? 0.001f: 0.0f, FLT_MAX, raycastHit))
		{
			// recursively scatter the ray
			glm::vec3 attenuation;
			Geom3D::Ray scatteredRay;
			if (raycastHit.hitMaterial->ScatterRay(raycastHit, attenuation, scatteredRay))
			{
				return attenuation * CalculatePixelColour(scatteredRay, recursionDepth + 1);
			}
		}

		return GetBackgroundColour(ray);
	}

	// raycast
	bool Raycast(const Geom3D::Ray& ray, float minDistance, float maxDistance, Geom3D::RaycastHit& raycastHit)
	{
    static MaterialDiffuse diffuse(glm::vec3(0.8f, 0.3f, 0.4f));

		Geom3D::Sphere sphere(glm::vec3(0.0f, 0.0f, -4.0f), 3.0f, &diffuse);
		bool raycast = sphere.Raycast(ray, minDistance, maxDistance, raycastHit);
		
    if (raycast)
    {
      raycastHit.ray = ray;
    }
   
		return raycast;
	}

	// get background colour
	glm::vec3 GetBackgroundColour(const Geom3D::Ray& ray)
	{
		// blend from white to blue
		glm::vec3 rayDirNormalized = glm::normalize(ray.Direction());
		float t = 0.5f*(rayDirNormalized.y + 1.0f);
		return (glm::vec3(1.0f, 1.0f, 1.0f)*(1.0f - t)) + (glm::vec3(0.5f, 0.7f, 1.0f)*t);
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

