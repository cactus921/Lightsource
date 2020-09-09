#include "pch.h"
#include "Beam.h"

Beam::Beam(double3 origin, double3 direction)
	:	//_scene {scene},
	_rays{std::vector<Ray*>()},
	_startingRay{ new Ray{origin, direction} }
{
	_rays.push_back(_startingRay);
	
}

Beam::~Beam()
{
}

void Beam::AddRay(Ray* ray)
{
	_rays.push_back(ray);
}

void Beam::GetBeamPoints(std::vector<double3>& out_points)
{
	out_points.clear();
	for (std::vector<int>::size_type i = 0; i != _rays.size(); i++)
	{
		out_points.push_back(_rays[i]->origin);
	}
}

void Beam::Clear()
{
	_rays.clear();
	_rays.push_back(_startingRay);
}


/*
SceneElement* Beam::CalculateIntersections()
{
	if (_rays.size() > 1)
	{
		// Iterate over all points in beam.
		for (std::vector<int>::size_type i = 0; i != _rays.size() - 1; i++)
		{
			// Iterate over all scene elements in scene.
			for (auto el = _scene->_sceneElements.begin(); el != _scene->_sceneElements.end(); el++)
			{
				if (_scene->Intersection(*_points[i], *_points[i + 1], *el))
				{
					return *el;
				}
			}
		}
	}
	
	return nullptr;
}
*/
