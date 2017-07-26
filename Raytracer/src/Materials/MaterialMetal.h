#ifndef MATERIAL_METAL
#define MATERIAL_METAL

#include "Material.h"
#include "glm/vec3.hpp"

class MaterialMetal : public Material
{

public:
	MaterialMetal(const glm::vec3& attenuation)
		: Material(attenuation)
	{
	};

	~MaterialMetal() {};

	// scatter ray
	bool ScatterRay(const Geom3D::RaycastHit& hitInfo, glm::vec3& attenuationOut, Geom3D::Ray& rayOut) const override
	{
		// reflected ray
		glm::vec3 rayInDirection = glm::normalize(hitInfo.ray.Direction());
		glm::vec3 reflected = rayInDirection - 2.0f*glm::dot(rayInDirection, hitInfo.hitNormal)*hitInfo.hitNormal;

		// set scattered ray and attenuation
		rayOut.Origin() = hitInfo.hitPos;
		rayOut.Direction() = reflected;
		attenuationOut = Attenuation();

		// check if it is scattered or otherwise absorbed
		bool scatter = glm::dot(reflected, hitInfo.hitNormal) > 0.0f;
		return scatter;
	}

private:

};

#endif // !MATERIAL_METAL
