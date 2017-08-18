#ifndef SHAPE_FACTORY_H
#define SHAPE_FACTORY_H

#include "Shapes.h"

namespace Geom3D
{
  struct ShapeFactoryParams
  {
    std::shared_ptr<Material> material = nullptr;

    std::string shapeType;
    glm::vec3 shapePos;
    float radius = 0.5f;
  };

  class ShapeFactory
  {
  public:
    
    static std::shared_ptr<Shape> Create(const ShapeFactoryParams& params)
    {
      std::shared_ptr<Shape> shape;
      if (params.shapeType == "Sphere")
      {
        shape = std::make_shared<Geom3D::Sphere>(params.shapePos, params.radius, params.material);
      }

      assert(shape);
      return shape;
    }

  private:

  };  
}

#endif
