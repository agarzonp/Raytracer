#ifndef SHAPE_H
#define SHAPE_H

#include "glm/vec3.hpp"

#include "../Ray.h"
#include "../AABB.h"

class Material;

namespace Geom3D
{
  struct RaycastHit
  {
    Ray ray;
		
		float hitDistance = FLT_MAX;
    glm::vec3 hitPos;
    glm::vec3 hitNormal;

    const Material* hitMaterial = nullptr;
  };

	class Shape
	{
  protected:

    // AABB
    AABB aabb;

	public:

		virtual ~Shape() {};

    // getters 
    const AABB& GetAABB() const { return aabb; }

    // Calculate AABB
    virtual void CalculateAABB() = 0;

    // Raycast
		virtual bool Raycast(const Ray& ray, float minDistance, float maxDistance, RaycastHit& raycastHit) = 0;

	};
}

#endif SHAPE_H
