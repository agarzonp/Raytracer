#ifndef MATERIAL_H
#define MATERIAL_H

#include "../Geom3D/Geom3D.h"

struct MaterialHitInfo
{
  const Geom3D::Ray& rayIn;
  const glm::vec3& hitPos;
  const glm::vec3& hitNormal;

  MaterialHitInfo(const Geom3D::Ray& rayIn_, const glm::vec3& hitPos_, const glm::vec3& hitNormal_)
    : rayIn(rayIn_)
    , hitPos(hitPos_)
    , hitNormal(hitNormal_)
  {
  }
};

class Material
{
  // attenuation
  glm::vec3 attenuation;

public:
 
  virtual ~Material() {};

  virtual bool ScatterRay(const MaterialHitInfo& materialHitInfo, glm::vec3& attenuationOut, Geom3D::Ray& rayOut) = 0;

  // getters/setters
  const glm::vec3& Attenuation() const { return attenuation; }
  glm::vec3& Attenuation() { return attenuation; }

protected:

  Material(const glm::vec3& attenuation_)
    :attenuation(attenuation_)
  {
  };

private:

  
};

#endif // !MATERIAL_H
