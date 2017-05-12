#ifndef SHAPE_H
#define SHAPE_H

#include "glm/vec3.hpp"

#include "../Ray.h"

class Material;

namespace Geom3D
{
  struct RaycastHit
  {
    Ray ray;
   
    glm::vec3 hitPos;
    glm::vec3 hitNormal;

    const Material* hitMaterial = nullptr;
  };

	class Shape
	{
	public:

		virtual ~Shape() {};

		virtual bool Raycast(const Ray& ray, RaycastHit& raycastHit) = 0;

	protected:

		Shape() {};
		
	private:

	};
}

#endif SHAPE_H
