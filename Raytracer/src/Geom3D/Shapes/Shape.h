#ifndef SHAPE_H
#define SHAPE_H

#include "glm/vec3.hpp"

#include "../Ray.h"

namespace Geom3D
{
	struct RaycastHit
	{
		glm::vec3 pos;
		glm::vec3 normal;
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
