#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"

#include <memory>

class Material;

namespace Geom3D
{
	class Sphere : public Shape
	{
    // center and radius
		glm::vec3 center;
		float radius;

    // attached material
    std::shared_ptr<Material> material;

	public:
		Sphere() 
		: center(glm::vec3(0.0f, 0.0f, 0.0f))
		, radius(1.0f)
		{
		};

		Sphere(const glm::vec3& center_, float radius_)
			: center(center_)
			, radius(radius_)
		{
		};

    Sphere(const glm::vec3& center_, float radius_, const std::shared_ptr<Material>& material_)
      : center(center_)
      , radius(radius_)
      , material(material_)
    {
    };

		~Sphere() {};

		// getters and setter
		const glm::vec3& Center() const { return center; }
		const float& Radius() const { return radius; }

		glm::vec3& Center() { return center; }
		float& Radius() { return radius; }

		const std::shared_ptr<Material>& GetMaterial() const { return material; }

		// Raycast
		bool Raycast(const Ray& ray, float minDistance, float maxDistance, RaycastHit& raycastHit) override
		{
			glm::vec3 cc = ray.Origin() - center;
			float a = glm::dot(ray.Direction(), ray.Direction());
			float b = glm::dot(cc, ray.Direction());
			float c = glm::dot(cc, cc) - radius*radius;
			float discriminant = b*b - a*c;

			if (discriminant > 0)
			{
				float discriminantSq = sqrtf(discriminant);
				float t1 = (-b - discriminantSq) / a;
				float t2 = (-b + discriminantSq) / a;

				float t = std::min(t1, t2);

				if (t >= minDistance && t <= maxDistance)
				{
					raycastHit.hitDistance = t;
					raycastHit.hitPos = ray.PointAtT(t);
					raycastHit.hitNormal = glm::normalize(raycastHit.hitPos - center);
					raycastHit.hitMaterial = material.get();

					return true;
				}
			}

			return false;
		}

	private:

	};
}

#endif
