#ifndef RAY_H
#define RAY_H

#include "glm/vec3.hpp"

namespace Geom3D
{
	class Ray
	{
		glm::vec3 o;
		glm::vec3 d;

	public:

		// constructors
		Ray()
			: o(glm::vec3())
			, d(glm::vec3(1.0f, 0.0f, 0.0f))

		{
		}

		Ray(const glm::vec3& origin, const glm::vec3& direction)
			: o(origin)
			, d(direction)
		{
		}

		// setter and getters
		glm::vec3& Origin() { return o; }
		glm::vec3& Direction() { return d; }

		const glm::vec3& Origin() const { return o; }
		const glm::vec3& Direction() const { return d; }

		// point at t
		glm::vec3 PointAtT(float t) { return o + t*d; }
	};
}

#endif // !RAY_H

