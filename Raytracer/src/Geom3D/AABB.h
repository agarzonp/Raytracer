#ifndef ABBB_H
#define ABBB_H

#include <algorithm>

namespace Geom3D
{
  class AABB 
  {
    glm::vec3 min;
    glm::vec3 max;

  public:
    AABB(){};
    AABB(const glm::vec3& min_, const glm::vec3& max_)
      : min(min_)
      , max(max_)
    {
    }

    // getters and setter
    glm::vec3& Min() { return min; }
    glm::vec3& Max() { return max; }

    const glm::vec3& Min() const { return min; }
    const glm::vec3& Max() const { return max; }

    // Intersect
    bool Intersect(const Ray& ray, float& tMin, float& tMax) const
    {
      // calculate minimum and maximum t on axis X
      float txMin = (min.x - ray.Origin().x) / ray.Direction().x;
      float txMax = (max.x - ray.Origin().x) / ray.Direction().x;
      if (txMin > txMax)
      {
        std::swap(txMin, txMax);
      }

      // calculate minimum and maximum t on Axis Y
      float tyMin = (min.y - ray.Origin().y) / ray.Direction().y;
      float tyMax = (max.y - ray.Origin().y) / ray.Direction().y;
      if (tyMin > tyMax)
      {
        std::swap(tyMin, tyMax);
      }

      if ((txMin > tyMax) || (tyMin > txMax))
      {
        // intersection outside the box
        return false;
      }
        
      // calculate minimum and maximum t on XY
      tMin = std::max(txMin, tyMin);
      tMax = std::min(txMax, tyMax);

      // calculate minimum and maximum t on Axis Z
      float tzMin = (min.z - ray.Origin().z) / ray.Direction().z;
      float tzMax = (max.z - ray.Origin().z) / ray.Direction().z;
      if (tzMin > tzMax)
      {
        std::swap(tzMin, tzMax);
      }

      if ((tMin > tzMax) || (tzMin > tMax))
      {
        // intersection outside the box
        return false;
      }
        
      // calculate minimum and maximum t on XYZ
      tMin = std::max(tMin, tzMin);
      tMax = std::min(tMax, tzMax);

      return true;
    }

  };
}

#endif // !ABBB_H

