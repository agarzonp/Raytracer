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

public:
	World() {};
	~World() {};

	// add shape
	std::shared_ptr<Geom3D::Shape> AddShape(std::shared_ptr<Geom3D::Shape> shape)
	{
		shapes.push_back(shape);
		return shapes.back();
	}

	// build BVH
	void BuildBVH()
	{
		bvh = BVH(shapes);
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
			}
		}
		
		return hit;
	}

private:

};

#endif // !WORLD_H

