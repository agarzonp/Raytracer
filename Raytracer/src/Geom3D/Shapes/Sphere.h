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

		// Raycast
		bool Raycast(const Ray& ray, RaycastHit& raycastHit) override
		{
			glm::vec3 cc = ray.Origin() - c;
			float a = glm::dot(ray.Direction(), ray.Direction());
			float b = glm::dot(cc, ray.Direction());
			float c = glm::dot(cc, cc) - r*r;
			float discriminant = b*b - a*c;

			if (discriminant > 0)
			{
				float discriminantSq = sqrtf(discriminant);
				float t1 = (-b - discriminantSq) / a;
				float t2 = (-b + discriminantSq) / a;

				float t = std::min(t1, t2);

				raycastHit.pos = ray.PointAtT(t);
				raycastHit.normal = glm::normalize(ray.Direction());

				return true;
			}

			return false;
		}

	private:

	};
}

#endif
