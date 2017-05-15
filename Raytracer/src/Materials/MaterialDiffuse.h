#ifndef MATERIAL_DIFFUSE
#define MATERIAL_DIFFUSE

#include <random>
#include "Material.h"

class MaterialDiffuse : public Material
{
  
public:
  MaterialDiffuse(const glm::vec3& attenuation)
    : Material(attenuation)
  {
  };

  ~MaterialDiffuse() {};

  // scatter ray
  bool ScatterRay(const Geom3D::RaycastHit& hitInfo, glm::vec3& attenuationOut, Geom3D::Ray& rayOut) const override
  {
		static std::random_device randomDevice;
		static std::mt19937 randomEngine(randomDevice());
		static std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);

		// calculate scattered ray direction by getting a random point in the unit sphere
		glm::vec3 randomPointInUnitSphere;
		do
		{
			randomPointInUnitSphere.x = distribution(randomEngine);
			randomPointInUnitSphere.y = distribution(randomEngine);
			randomPointInUnitSphere.z = distribution(randomEngine);

		} while (randomPointInUnitSphere.length() <= 1.0f);
    
		glm::vec3 unitSphereCenter = hitInfo.hitPos + hitInfo.hitNormal;
		glm::vec3 target = unitSphereCenter + randomPointInUnitSphere;

    // set scattered ray and attenuation
    rayOut.Origin() = hitInfo.hitPos;
    rayOut.Direction() = target - hitInfo.hitPos;
    attenuationOut = Attenuation();

    return true;
  }

private:

};

#endif // !MATERIAL_DIFFUSE
