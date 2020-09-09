#include "pch.h"
#include "SceneElement.h"

/*
SceneElement::SceneElement()
	: 
	_position{double3(0.0)},
	_rotation{double4(1.0, 0.0, 0.0, 0.0)}
{
}
*/

SceneElement::SceneElement(double3 position, double3 normal)
	: _position{ position },
	_normal{ normal },
	_radius{ 2.0 },
	_plane{ position, normal}
{
}

SceneElement::~SceneElement()
{
}

void SceneElement::SetTransform(double3 position, double3 normal)
{
	_position = position;
	_normal = normal;
	_plane.originPoint = position;
	_plane.normal = normal;
}

/*
double3 SceneElement::CalcRayIntersectionPoint(Ray* ray)
{
	return double3();
}
*/


/*  This calculates intersection with plane - assumes always intersects. */
double SceneElement::CalcRayIntersectionDistance(Ray* ray)
{
	double3 p0 = _plane.originPoint;
	double3 n = linalg::normalize(_plane.normal);
	double3 l0 = ray->origin;
	double3 l = linalg::normalize( ray->direction );
	double d = linalg::dot(p0 - l0, n) / linalg::dot(l, n);
	/* TODO:
	- Move this to plane struct?
	- Check if intersection is within element bounds.
	- if not, return -1
	*/
	return d;
}

/* This function is to be overwritten by the derived classes.*/
bool SceneElement::IsLocationWithinElement(double3 location)
{
	double d = linalg::abs(linalg::dot((location - _plane.originPoint), _plane.normal));
	bool isOnPlane = d <= 0.0001;

	// This assumes a circular element - determine by distance from origin.
	bool isWithinElement = linalg::distance(location, _plane.originPoint) <= _radius;
	
	return isOnPlane && isWithinElement;
}

/* This function is to be overwritten by the derived classes.*/
Ray* SceneElement::CalcOutRay(Ray* incoming_ray, double3 location)
{
	// This assumes a mirror element.
	double3 ref_point_in = location + incoming_ray->direction;
	double3 ref_to_plane = abs( linalg::dot(incoming_ray->direction, _plane.normal) ) * _plane.normal;
	double3 ref_point_out = ref_point_in + 2 * ref_to_plane;
	double3 direction_out = linalg::normalize( ref_point_out - location );
	return new Ray{ location, direction_out };
}
