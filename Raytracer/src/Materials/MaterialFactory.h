#ifndef MATERIAL_FACTORY_H
#define MATERIAL_FACTORY_H

#include "Materials.h"

struct MaterialFactoryParams
{
  std::string materialType;
  glm::vec3 materialColour;
};

class MaterialFactory
{
public:

  static std::shared_ptr<Material> Create(const MaterialFactoryParams& params)
  {
    std::shared_ptr<Material> material;
    if (params.materialType == "Diffuse")
    {
      material = std::make_shared<MaterialDiffuse>(params.materialColour);
    }
    else if (params.materialType == "Metal")
    {
      material = std::make_shared<MaterialMetal>(params.materialColour);
    }

    assert(material);

    return material;
  }
};

#endif // !MATERIAL_FACTORY_H

