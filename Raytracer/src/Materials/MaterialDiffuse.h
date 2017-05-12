#ifndef MATERIAL_DIFFUSE
#define MATERIAL_DIFFUSE

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
  bool ScatterRay(const Geom3D::RaycastHit& hitInfo, glm::vec3& attenuationOut, Geom3D::Ray& rayOut) override
  {
    glm::vec3 offset(0.0f, 0.0f, 0.0f); // TO-DO: add some random offset between [-1.0f, 1.0f] in the unit sphere
    glm::vec3 target = hitInfo.hitPos + hitInfo.hitNormal + offset;

    // set scattered ray and attenuation
    rayOut.Origin() = hitInfo.hitPos;
    rayOut.Direction() = target - hitInfo.hitPos;
    attenuationOut = Attenuation();

    return true;
  }

private:

};

#endif // !MATERIAL_DIFFUSE
