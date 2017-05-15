#ifndef WORLD_H
#define	WORLD_H

#include <memory>
#include <list>
#include "../Geom3D/Geom3D.h"

class World
{
	// shapes
	std::list<std::shared_ptr<Geom3D::Shape>> shapes;

public:
	World() {};
	~World() {};

	// add shape
	std::shared_ptr<Geom3D::Shape> AddShape(std::shared_ptr<Geom3D::Shape> shape)
	{
		shapes.push_back(shape);
		return shapes.back();
	}

	// raycast
	bool Raycast(const Geom3D::Ray& ray, float minDistance, float maxDistance, Geom3D::RaycastHit& raycastHit)
	{
		Geom3D::RaycastHit tempHit;
		bool hit = false;

		for (auto& shape : shapes)
		{
			if (shape->Raycast(ray, minDistance, maxDistance, tempHit) && tempHit.hitDistance < raycastHit.hitDistance)
			{
				raycastHit = tempHit;
				hit = true;
			}
		}

		return hit;
	}

private:

};

#endif // !WORLD_H

