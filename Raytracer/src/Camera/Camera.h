#ifndef CAMERA_H
#define CAMERA_H

#include<cmath>

#include "glm/vec3.hpp"

#include "../Geom3D/Geom3D.h"

class Camera
{
	// field of view (degrees)
	float verticalFieldOfView = 45.0f;

	// aspect ratio
	float aspectRatio = 1.33f;

  // near plane
  glm::vec3 nearPlaneBottomLeft;
	glm::vec3 nearPlaneHorizontal;
	glm::vec3 nearPlaneVertical;

  // camera position
  glm::vec3 position;

public:

  // constructors
	Camera() 
	{
		Init(position, glm::vec3(0.0f, 0.0f, -1.0f), verticalFieldOfView, aspectRatio);
	}

	Camera(const glm::vec3& pos, const glm::vec3& lookAt, float verticalFov, float aspect)
	{
		Init(pos, lookAt, verticalFov, aspect);
	}

  ~Camera() {};

	// init
	void Init(const glm::vec3& pos, const glm::vec3& lookAt, float verticalFov, float aspect)
	{
		position = pos;
		verticalFieldOfView = verticalFov;
		aspectRatio = aspect;

		float vFovRadians = glm::radians(verticalFieldOfView);
		float halfHeight = tanf(vFovRadians*0.5f);
		float halfWidth = halfHeight * aspectRatio;
		float height = halfHeight * 2;
		float width = halfWidth * 2;

		glm::vec3 forward = glm::normalize(position - lookAt);
		glm::vec3 right = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), forward);
		glm::vec3 up = glm::cross(forward, right);

		nearPlaneHorizontal = right * width;
		nearPlaneVertical = up * height;
		nearPlaneBottomLeft = position - (right * halfWidth) - (up * halfHeight) - forward;
	}

  // getters/setters
  const glm::vec3& Position() const { return position; }
  glm::vec3& Position() { return position; }

  // get ray
  Geom3D::Ray GetRay(float u, float v)
  {
		glm::vec3 target = nearPlaneBottomLeft + nearPlaneHorizontal * u + nearPlaneVertical * v - position;
    return Geom3D::Ray(position, target - position);
  }

private:

};


#endif
