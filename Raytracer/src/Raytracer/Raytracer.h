#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <chrono>
#include <random>
#include <sstream>

#include "../ThreadPool/ThreadPool.h"

#include "../Geom3D/Geom3D.h"

#include "Camera/Camera.h"
#include "Materials/MaterialFactory.h"

#define PROFILE_HIT_TEST 0
#include "../World/World.h"

typedef std::chrono::time_point<std::chrono::system_clock> TimePoint;

// random number generation
std::random_device randomDevice;
std::mt19937 randomEngine(randomDevice());
std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

std::uniform_real_distribution<float> spherePositionXDistribution(-2.0f, 2.0f);
std::uniform_real_distribution<float> spherePositionYDistribution(-1.0f, 0.0f);
std::uniform_real_distribution<float> spherePositionZDistribution(-10.0f, 5.0f);
std::uniform_real_distribution<float> sphereRadiusDistribution(0.01f, 0.1f);
std::uniform_real_distribution<float> materialAttenuationDistribution(0.0f, 1.0f);
std::uniform_int_distribution<int>  materialTypeDistribution(0, 4);

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

  int antialiasingSamplesCount = 1;
	int maxRecursionDepth = 1;
	int renderingSubtasksCount = 1;
	bool useBVH = false;
	
  int randomShapes = 0;
  std::string sceneId;
};

class Raytracer
{
	// width and height
	int width = -1;
	int height = -1;

  // antialiasing samples
  int antialiasingSamplesCount = 1;

	// max recursion depth
	int maxRecursionDepth = 1;

	// use Bounding Volume Hierarchy optimisation
	bool useBVH = false;

	// pixels buffer
	float* buffer = nullptr;

	// raytracer state
	RaytracerState state = RaytracerState::IDLE;

	// to keep track of rendering time
	TimePoint renderStart;
	std::string renderTimeStr;

	// threadpool
	ThreadPool threadPool;
	int renderingSubtasksCount = 1;

	// camera
	Camera camera;

	// world
	World world;

public:
	
	// Singleton instance
	static Raytracer& Get()
	{
		static Raytracer instance;
		return instance;
	}

  // is rendering
  bool IsRendering() { return state == RaytracerState::RENDERING; }

  // setters
  void SetAntialiasingSamplesCount(unsigned count) { antialiasingSamplesCount = count; }
  void SetMaxRecursionDepth(unsigned depth) { maxRecursionDepth = depth; }
  void SetRenderingSubtasksCount(unsigned count) { renderingSubtasksCount = count; }
  void SetUseBVH(bool use) { useBVH = use; }

	// init
	void Init(const RaytracerConfiguration& config)
	{
		width = config.width;
		height = config.height;
		buffer = config.buffer;

    SetAntialiasingSamplesCount(config.antialiasingSamplesCount);
    SetMaxRecursionDepth(config.maxRecursionDepth);
    SetRenderingSubtasksCount(config.renderingSubtasksCount);
    SetUseBVH(config.useBVH);

    InitCamera();

		LoadScene(config);
	}

	// start rendering
	void StartRendering()
	{
		if (state == RaytracerState::IDLE)
		{
			state = RaytracerState::RENDERING;

			#if PROFILE_HIT_TEST
			world.ResetHitTestCount();
			#endif		

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
		
		if (renderingSubtasksCount > 1)
		{
			// Split rendering by adding as many render tasks as renderingSubtasksCount
			// Each thread will be responsible for doing a render chunk
			std::vector<ThreadTaskResult> taskResults;

			int heightInterval = height / renderingSubtasksCount;
			for (int i = 0; i < renderingSubtasksCount; i++)
			{
				int startHeight = height - (i * heightInterval);
				int endHeight = (i == renderingSubtasksCount - 1) ? 0 : startHeight - heightInterval;

				auto task = std::bind(&Raytracer::RenderChunk, this, startHeight, endHeight);
				auto taskResult = threadPool.AddTask(task);

				taskResults.push_back(std::move(taskResult));
			}

			// make current thread to wait until all render tasks has been completed
			void* result = nullptr;
			for (size_t i = 0; i < taskResults.size(); i++)
			{
				taskResults[i].WaitForResult(result);
			}
		}
		else
		{
			RenderChunk(height, 0);
		}
		
		// notify render ended
		bool cancelled = (state == RaytracerState::RENDERING_CANCELLED);
		OnRenderingEnded(cancelled);
	}

protected:
	Raytracer() : state(RaytracerState::IDLE) {}
	~Raytracer() {};

	// internal render
	void RenderChunk(int startHeight, int endHeight)
	{
		for (int y = startHeight - 1; y >= endHeight; y--)
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
					return;
				}
			}
		}
	}

  // calculate pixel colour
  inline glm::vec4 CalculatePixelColour(int x, int y, Camera& camera)
  {
    // Note: we send more than one ray per pixel (randomly offset) in order to do antialiasing

    glm::vec3 pixelColour(0.0f, 0.0f, 0.0f);
    for (int sample = 0; sample < antialiasingSamplesCount; sample++)
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
    pixelColour /= float(antialiasingSamplesCount);

    return glm::vec4(pixelColour, 1.0f);
  }

	// calculate pixel colour
	glm::vec3 CalculatePixelColour(const Geom3D::Ray& ray, int recursionDepth)
	{
		// raycast
		Geom3D::RaycastHit raycastHit;
		if (Raycast(ray, recursionDepth > 0 ? 0.001f: 0.0f, FLT_MAX, raycastHit))
		{
			// recursively scatter the ray
			glm::vec3 attenuation;
			Geom3D::Ray scatteredRay;
			if (recursionDepth < maxRecursionDepth && raycastHit.hitMaterial->ScatterRay(raycastHit, attenuation, scatteredRay))
			{
				return attenuation * CalculatePixelColour(scatteredRay, recursionDepth + 1);
			}
			else
			{
				return glm::vec3(0.0f, 0.0f, 0.0f);
			}
		}

		return GetBackgroundColour(ray);
	}

	// raycast
	bool Raycast(const Geom3D::Ray& ray, float minDistance, float maxDistance, Geom3D::RaycastHit& raycastHit)
	{
		bool raycast = world.Raycast(ray, minDistance, maxDistance, raycastHit);
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
	
	// load scene
	void LoadScene(const RaytracerConfiguration& config)
	{
    // clear current world
    world.Clear();

    // load the scene defined or a random one
    config.sceneId.empty() ? CreateRandomScene(config.randomShapes) : LoadScene(config.sceneId);

    // build BVH
		if (useBVH)
		{
			world.BuildBVH();
		}
	}

  void LoadScene(const std::string& sceneId)
  {
    // TO-DO

    MaterialFactoryParams materialParams;
    materialParams.materialType = "Diffuse";
    materialParams.materialColour = glm::vec3(0.8f, 0.3f, 0.4f);
    std::shared_ptr<Material> material = MaterialFactory::Create(materialParams);

    Geom3D::ShapeFactoryParams shapeParams;
    shapeParams.shapeType = "Sphere";
    shapeParams.shapePos = glm::vec3(-0.5f, 0.0f, -1.3f);
    shapeParams.radius = 0.5f;
    shapeParams.material = material;
    std::shared_ptr<Geom3D::Shape> shape = Geom3D::ShapeFactory::Create(shapeParams);
    
    world.AddShape(shape);
  }

  // create random scene
	void CreateRandomScene(int randomShapes)
	{
    // two big spheres
    world.AddShape(CreateSphere(glm::vec3(-0.5f, 0.0f, -1.3f), 0.5f, "Diffuse", glm::vec3(0.8f, 0.3f, 0.4f)));
    world.AddShape(CreateSphere(glm::vec3(0.7f, 0.0f, -3.0f), 0.5f, "Metal", glm::vec3(0.8f, 0.6f, 0.2f)));

    // floor 
    world.AddShape(CreateSphere(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f, "Diffuse", glm::vec3(0.8f, 0.8f, 0.8f)));

		// random shapes
		for (int i = 0; i < randomShapes; i++)
		{
			// random material
			glm::vec3 attenuation(materialAttenuationDistribution(randomEngine), materialAttenuationDistribution(randomEngine), materialAttenuationDistribution(randomEngine));
			bool diffuseMaterial = materialTypeDistribution(randomEngine) > 0;
			
			// random sphere
			glm::vec3 spherePos(spherePositionXDistribution(randomEngine), spherePositionYDistribution(randomEngine), spherePositionZDistribution(randomEngine));
			float sphereRadius = sphereRadiusDistribution(randomEngine);

      world.AddShape(CreateSphere(spherePos, sphereRadius, diffuseMaterial ? "Diffuse" : "Metal", attenuation));
		}
	}

  std::shared_ptr<Geom3D::Shape> CreateSphere(const glm::vec3& pos, float radius, const std::string& materialType, const glm::vec3& materialColour)
  {
    MaterialFactoryParams materialParams;
    materialParams.materialType = materialType;
    materialParams.materialColour = materialColour;
    std::shared_ptr<Material> material = MaterialFactory::Create(materialParams);

    Geom3D::ShapeFactoryParams shapeParams;
    shapeParams.shapeType = "Sphere";
    shapeParams.shapePos = pos;
    shapeParams.radius = radius;
    shapeParams.material = material;

    return Geom3D::ShapeFactory::Create(shapeParams);
  }

	// init camera
	void InitCamera()
	{
		// camera
		float aspect = float(width) / float(height);
		float verticalFieldOfView = 90.0f;
		camera.Init(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -0.1f), verticalFieldOfView, aspect);
	}

	// on rendering ended
	void OnRenderingEnded(bool cancelled)
	{
		// output render ended status and time
		renderTimeStr = GetTimeStr(renderStart, std::chrono::system_clock::now());
		printf("Rendering %s!. Render took: %s\n", cancelled ? "CANCELLED" : "DONE", renderTimeStr.c_str());

#if PROFILE_HIT_TEST
		printf("Hit test count: %llu\n", world.GetHitTestCount());
#endif

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

