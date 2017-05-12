#ifndef MATERIAL_H
#define MATERIAL_H

#include "../Geom3D/Geom3D.h"

class Material
{
  // attenuation
  glm::vec3 attenuation;

public:
 
  virtual ~Material() {};

  virtual bool ScatterRay(const Geom3D::RaycastHit& hitInfo, glm::vec3& attenuationOut, Geom3D::Ray& rayOut) = 0;

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
