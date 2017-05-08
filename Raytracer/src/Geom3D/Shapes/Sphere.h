#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"

namespace Geom3D
{
	class Sphere : public Shape
	{
		glm::vec3 c;
		float r;

	public:
		Sphere() 
		: c(glm::vec3(0.0f, 0.0f, 0.0f))
		, r(1.0f)
		{
		};

		Sphere(const glm::vec3& center, float radius)
			: c(center)
			, r(radius)
		{
		};

		~Sphere() {};

		// getters and setter
		const glm::vec3& Center() const { return c; }
		const float& Radius() const { return r; }

		glm::vec3& Center() { return c; }
		float& Radius() { return r; }

		// Intersect
		bool Intersect(const Ray& ray) override
		{
			glm::vec3 cc = ray.Origin() - c;
			float a = glm::dot(ray.Direction(), ray.Direction());
			float b = 2.0f * glm::dot(cc, ray.Direction());
			float c = glm::dot(cc, cc) - r*r;
			float discriminant = b*b - 4*a*c;

			return (discriminant > 0);
		}

	private:

	};
}

#endif
