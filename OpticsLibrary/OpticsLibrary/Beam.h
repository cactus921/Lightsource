#pragma once
#include <vector>
#include "Linalg.h"
using namespace linalg::aliases;

struct Ray
{
	double3 origin;
	double3 direction;
};

class Beam
{
public:
	Beam(double3 origin, double3 direction);
	~Beam();

	void AddRay(Ray* ray);
	inline int GetNumPoints() { return (int)(_rays.size()); };
	void GetBeamPoints(std::vector<double3>& out_points);
	void Clear();
	std::vector<Ray*> _rays;

private:
	
	Ray* _startingRay;
};

