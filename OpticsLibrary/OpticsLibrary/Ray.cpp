#include "pch.h"
#include "Ray.h"

Ray::Ray(double3 position, double3 direction)
	:	_startPosition {position},
		_direction {direction}
{
}

Ray::~Ray()
{
}

SceneElement* Ray::CalcIntersection(std::vector<SceneElement*>& elements)
{
	SceneElement* closestElement = nullptr;
	double minD = 0.0, d = 0.0;
	for (std::vector<int>::size_type i = 0; i != elements.size(); i++)
	{
		d = elements[i]->IsRayIntersectingElement(this);
		if (d > 0.0 && d < minD )
		{
			closestElement = elements[i];
			minD = d;
		}
	}
	return nullptr;
}
