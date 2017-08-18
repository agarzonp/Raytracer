#ifndef WORLD_H
#define	WORLD_H

#include <memory>
#include <vector>
#include "../Geom3D/Geom3D.h"
#include "../Raytracer/BVH.h"

class World
{
	// shapes
	std::vector<std::shared_ptr<Geom3D::Shape>> shapes;

	// bounding volume hierarchy
	BVH bvh;
	
	// flag to use the BVH for raycasting
	bool useBVH = false;

#if PROFILE_HIT_TEST
	// tracks the number of hit test done
	std::atomic<uint64_t> hitTestCount = 0;
#endif // PROFILE_HIT_TEST


public:
	World() {};
	~World() {};

  // clear
  void Clear()
  {
    shapes.clear();
  }

	// add shape
	std::shared_ptr<Geom3D::Shape> AddShape(std::shared_ptr<Geom3D::Shape> shape)
	{
    if (shape)
    {
      shapes.push_back(shape);
      return shapes.back();
    }
		
    return nullptr;
	}

	// build BVH
	void BuildBVH()
	{
		bvh.Build(shapes);
		useBVH = true;
	}

	// raycast
	bool Raycast(const Geom3D::Ray& ray, float minDistance, float maxDistance, Geom3D::RaycastHit& raycastHit)
	{
		Geom3D::RaycastHit tempHit;
		bool hit = false;

		if (useBVH)
		{
			hit = bvh.Raycast(ray, minDistance, maxDistance, raycastHit);
		}
		else
		{
			// brute force
			for (auto& shape : shapes)
			{
				if (shape->Raycast(ray, minDistance, maxDistance, tempHit) && tempHit.hitDistance < raycastHit.hitDistance)
				{
					raycastHit = tempHit;
					hit = true;
				}

				#if PROFILE_HIT_TEST
				hitTestCount++;
				#endif
			}
		}
		
		return hit;
	}


#if PROFILE_HIT_TEST
	// Hit test count
	void ResetHitTestCount()
	{
		hitTestCount = 0;

		if (useBVH)
		{
			bvh.ResetHitTestCount();
		}

	}
	uint64_t GetHitTestCount() 
	{ 
		return  useBVH ? bvh.GetHitTestCount() : hitTestCount; 
	}
#endif

private:

};

#endif // !WORLD_H

