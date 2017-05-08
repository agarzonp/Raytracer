#ifndef SHAPE_H
#define SHAPE_H

#include "glm/vec3.hpp"

#include "../Ray.h"

namespace Geom3D
{
	class Shape
	{
	public:

		virtual ~Shape() {};

		virtual bool Intersect(const Ray& ray) = 0;

	protected:

		Shape() {};
		
	private:

	};
}

#endif SHAPE_H
