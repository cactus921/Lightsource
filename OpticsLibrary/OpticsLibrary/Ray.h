#pragma once
//#include "Scene.h"
#include <vector>
//#include "SceneElement.h"
#include "Linalg.h"
using namespace linalg::aliases;

class SceneElement;

class Ray
{
public:
	Ray(double3 position, double3 direction);
	~Ray();

	inline double3 GetStartPosition() { return _startPosition; };
	inline double3 GetDirection() { return _direction; };
	SceneElement* CalcIntersection(std::vector<SceneElement*> &elements);

private:
	double3 _startPosition;
	double3 _direction;
};

