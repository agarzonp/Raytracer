#ifndef CAMERA_H
#define CAMERA_H

#include "glm/vec3.hpp"

#include "Geom3D/Geom3D.h"

class Camera
{
  // near plane
  float nearPlaneWidth = -1;
  float nearPlaneHeight = -1;
  glm::vec3 nearPlaneBottomLeft;

  // camera position
  glm::vec3 position;

public:

  // constructors
  Camera(int width, int height)
  {
    nearPlaneWidth = float(width) / 100.0f;
    nearPlaneHeight = float(height) / 100.0f;
    nearPlaneBottomLeft = glm::vec3(-nearPlaneWidth*0.5f, -nearPlaneHeight*0.5f, -1.0f);

    // camera position
    position = glm::vec3(0.0f, 0.0f, 0.0f);
  };

  ~Camera() {};

  // getters/setters
  const glm::vec3& Position() const { return position; }
  glm::vec3& Position() { return position; }

  // get ray
  Geom3D::Ray GetRay(float u, float v)
  {
    glm::vec3 rayDir = nearPlaneBottomLeft + glm::vec3(u * nearPlaneWidth, 0.0f, 0.0f) + glm::vec3(0.0f, v*nearPlaneHeight, 0.0f);
    return Geom3D::Ray(position, rayDir);
  }

private:

};


#endif
