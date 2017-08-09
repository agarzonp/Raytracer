#ifndef BVH_H
#define BVH_H

#include <cassert>
#include "../Geom3D/Geom3D.h"


class BVH : public Geom3D::Shape
{
	// pointer to children
	std::shared_ptr<Geom3D::Shape> left;
	std::shared_ptr<Geom3D::Shape> right;

public:

	// constructors
	BVH() 
	{
	}

	BVH(std::vector<std::shared_ptr<Geom3D::Shape>>& shapes)
	{
		// Recursively Build BVH
		RecursivelyBuildBVH(shapes);
	}

	// calculate AABB
	void CalculateAABB()
	{
		// the bounding box tha contains left and right AABB

		auto& leftMin = left->GetAABB().Min();
		auto& rightMin = right->GetAABB().Min();

		aabb.Min() = glm::vec3(std::fminf(leftMin.x, rightMin.x), std::fminf(leftMin.y, rightMin.y), std::fminf(leftMin.z, rightMin.z));

		auto& leftMax = left->GetAABB().Max();
		auto& rightMax = right->GetAABB().Max();

		aabb.Max() = glm::vec3(std::fmaxf(leftMax.x, rightMax.x), std::fmaxf(leftMax.y, rightMax.y), std::fmaxf(leftMax.z, rightMax.z));
	}

	// Raycast
	bool Raycast(const Geom3D::Ray& ray, float minDistance, float maxDistance, Geom3D::RaycastHit& raycastHit) const override
	{
		// Raycast with children if we only intersect current aabb
		float tMin = 0.0f;
		float tMax = 0.0f;
		if (!aabb.Intersect(ray, tMin, tMax))
		{
			// No intersection so no need to check children
			return false;
		}

		// Left Raycast
		Geom3D::RaycastHit leftRaycastHit;
		bool hitLeft = left->Raycast(ray, minDistance, maxDistance, leftRaycastHit);

		// Right Raycast
		Geom3D::RaycastHit rightRaycastHit;
		bool hitRight = right->Raycast(ray, minDistance, maxDistance, rightRaycastHit);

		if (hitLeft && hitRight)
		{
			// return the closest hit
			raycastHit = leftRaycastHit.hitDistance < rightRaycastHit.hitDistance ? leftRaycastHit : rightRaycastHit;
			return true;
		}

		if (hitLeft)
		{
			// return left
			raycastHit = leftRaycastHit;
			return true;
		}
		
		if (hitRight)
		{
			// return right
			raycastHit = rightRaycastHit;
			return true;
		}

		return false;
	}

private:

	// Build BVH
	void RecursivelyBuildBVH(std::vector<std::shared_ptr<Geom3D::Shape>>& shapes)
	{
		// Set children
		SetChildren(shapes);

		// Calculate AABB
		CalculateAABB();
	}

	// Set children
	void SetChildren(std::vector<std::shared_ptr<Geom3D::Shape>>& shapes)
	{
		switch (shapes.size())
		{
		case 0:
			assert(false);
			break;
		case 1:
			// make both point to single shape
			left = right = shapes[0];
			break;
		case 2:
			// make the left to point to the first and right to the second
			left = shapes[0];
			right = shapes[1];
			break;
		default:
		{
			// recursively construct a new BVH that holds half of the shapes
			unsigned halfShapes = shapes.size() / 2;

			std::vector<std::shared_ptr<Geom3D::Shape>> leftShapes;
			for (unsigned i = 0; i < halfShapes; i++)
			{
				leftShapes.emplace_back(shapes[i]);
			}
			left = std::make_shared<BVH>(leftShapes);

			std::vector<std::shared_ptr<Geom3D::Shape>> rightShapes;
			for (unsigned i = halfShapes; i < shapes.size(); i++)
			{
				rightShapes.emplace_back(shapes[i]);
			}
			right = std::make_shared<BVH>(rightShapes);
			break;
		}
		}
	}

};

#endif // !BVH_H

