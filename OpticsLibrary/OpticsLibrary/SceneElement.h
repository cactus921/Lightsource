#pragma once
#include <iostream>
#include "Linalg.h"
#include "Beam.h"
using namespace linalg::aliases;

#ifdef _DEBUG
#define DPRINT(x) std::cout << x << std::endl
#else
#define DPRINT(x)
#endif

enum ELEMENT_TYPE {
	Wall,
	Mirror,
	Lens,
	Prism,
	Sensor
};

struct Plane {
	double3 originPoint;
	double3 normal;
};


class SceneElement
{
public:
	//SceneElement();
	SceneElement(double3 position, double3 normal);
	~SceneElement();

	void SetTransform(double3 position, double3 normal);
	//double3 CalcRayIntersectionPoint(Ray* ray);
	//double IsRayIntersectingElement(Ray* ray);
	double CalcRayIntersectionDistance(Ray* ray);
	bool IsLocationWithinElement(double3 location);

	Ray* CalcOutRay(Ray* incoming_ray, double3 location);


	double3 _position;
	double3 _normal;
	double _radius;
	Plane _plane;

private:

};

