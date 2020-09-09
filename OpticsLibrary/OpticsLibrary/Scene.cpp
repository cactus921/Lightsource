#include "pch.h"
#include "Scene.h"


Scene::Scene()
{
	//For Debugging
#ifdef _DEBUG
	FILE* pConsole;
	AllocConsole();
	freopen_s(&pConsole, "CONOUT$", "wb", stdout);
	DPRINT("************* START RUN **************");
	DPRINT("Created Scene: " << this);
#endif // 
}

Scene::~Scene()
{
}


Beam* Scene::CreateBeam(double3 position, double3 direction)
{
	Beam* b = new Beam(position, direction);
	_beam = b;
	DebugLog("Created Beam at: ", position);

	TraceBeam(b);

	return b;
}

SceneElement* Scene::CreateSceneElement(double3 position, double3 normal)
{
	SceneElement* el = new SceneElement(position, normal);
	_sceneElements.push_back(el);
	DebugLog("Created Element at: ", position);

	TraceBeam(_beam);

	return el;
}

void Scene::SetSceneElementTransform(SceneElement* element, double3 position, double3 normal)
{
	/* TODO : check if element exists in scene. */
	element->SetTransform(position, normal);

	TraceBeam(_beam);
}

void Scene::GetBeamPoints(Beam* beam, std::vector<double3> &out_points)
{
	beam->GetBeamPoints(out_points);
}



IntersectionInformation Scene::NearestIntersection(Ray* ray)
{
	SceneElement* closestElement = nullptr;
	double minD = 1000.0, d = 0.0;
	double3 intersectLoaction;
	for (std::vector<int>::size_type i = 0; i != _sceneElements.size(); i++)
	{
		d = _sceneElements[i]->CalcRayIntersectionDistance(ray);
		intersectLoaction = ray->origin + linalg::normalize(ray->direction) * d;
		if (_sceneElements[i]->IsLocationWithinElement(intersectLoaction) && d > 0.0001 && d < minD)
		{
			closestElement = _sceneElements[i];
			minD = d;
		}
	}
	intersectLoaction = ray->origin + linalg::normalize(ray->direction) * minD;
	Ray* outRay = nullptr;
	if (closestElement != nullptr)
	{
		outRay = closestElement->CalcOutRay(ray, intersectLoaction);
	}

	return IntersectionInformation
	{
		closestElement,
		intersectLoaction,
		outRay
	};
		
}

void Scene::TraceBeam(Beam* beam)
{
	beam->Clear();
	Ray* currentRay = beam->_rays[0];
	IntersectionInformation info = NearestIntersection(currentRay);
	while(info.outRay != nullptr)
	{
		currentRay = info.outRay;
		beam->AddRay(currentRay);
		info = NearestIntersection(currentRay);		
	}
	beam->AddRay(new Ray{ info.worldPoint, double3(0.0) });
}

/*
void Scene::AddSceneElement(ELEMENT_TYPE type, double3 position, double4 rotation)
{
	//TODO : switch on type of element.
	//_sceneElements.push_back(new SceneElement(this, position, rotation));
}
*/

void Scene::DebugLog(const std::string str)
{
	DPRINT(str);
}
void Scene::DebugLog(const std::string str, const double value)
{
	DPRINT(str << ": " << value);
}
void Scene::DebugLog(const std::string str, const double3 value)
{
	DPRINT(str << ": " << value.x << ", " << value.y << ", " << value.z);
}

/************************************  Exported Methods  ******************************************/

Scene* ExtCreateNativeScene()
{
	//return &Scene::getInstance();
	return new Scene();
}

Beam* ExtCreateNativeBeam(Scene* scene, double position_arr_in[3], double direction_arr_in[3])
{
	double3 position = double3(position_arr_in[0], position_arr_in[1], position_arr_in[2]);
	double3 direction = double3(direction_arr_in[0], direction_arr_in[1], direction_arr_in[2]);
	return scene->CreateBeam(position, direction);
}

SceneElement* ExtCreateNativeSceneElement(Scene* scene, double position_arr_in[3], double normal_arr_in[3])
{
	double3 position = double3(position_arr_in[0], position_arr_in[1], position_arr_in[2]);
	double3 normal = double3(normal_arr_in[0], normal_arr_in[1], normal_arr_in[2]);
	return scene->CreateSceneElement(position, normal);
}

void ExtSetElementTransform(Scene* scene, SceneElement* element, double position_arr_in[3], double normal_arr_in[3])
{
	double3 position = double3(position_arr_in[0], position_arr_in[1], position_arr_in[2]);
	double3 normal = double3(normal_arr_in[0], normal_arr_in[1], normal_arr_in[2]);
	scene->SetSceneElementTransform(element, position, normal);
}

void ExtGetBeamPoints(Scene* scene, Beam* beam, double beam_points_out[3 * MAX_BEAM_POINTS])
{
	std::vector<double3> points_vec;
	scene->GetBeamPoints(beam, points_vec);

	size_t limit = points_vec.size();
	if (limit > MAX_BEAM_POINTS)
	{
		DPRINT("WARNING: Number of points per beam exceeded maximum: " << points_vec.size());
		limit = (size_t)MAX_BEAM_POINTS;
	}

	for (std::vector<int>::size_type i = 0; i != limit; i++)
	{
		//scene->DebugLog("point", points_vec[i]);
		beam_points_out[i * 3] = points_vec[i].x;
		beam_points_out[i * 3 + 1] = points_vec[i].y;
		beam_points_out[i * 3 + 2] = points_vec[i].z;
	}

	for (int k = limit * 3 ; k < MAX_BEAM_POINTS * 3; k++)
	{
		beam_points_out[k] = beam_points_out[k-3];
	}
}











/*
void GetBeamPoints(Scene* scene, Beam* beam, SAFEARRAY*& sarr_in)
{
	SafeArrayDestroy(sarr_in);
	sarr_in = NULL;
	
	SAFEARRAYBOUND saBound;
	saBound.lLbound = 0;
	saBound.cElements = beam->GetNumPoints() * 3;
	DPRINT("NumPoints: " << beam->GetNumPoints());
	DPRINT("Arr Size: " << saBound.cElements);

	sarr_in = SafeArrayCreate(VT_R8, 1, &saBound);
	double* pVals;

	HRESULT hr = SafeArrayAccessData(sarr_in, (void**)& pVals); // direct access to SA memory

	if (SUCCEEDED(hr))
	{
		for (ULONG i = 0; i < saBound.cElements; i++)
		{
			pVals[i] = i + 100.0;
			DPRINT(pVals[i]);
			//TODO : Populate array.
		}
	}
	else
	{
		// Error
	}


	// Read back the array to check.
	VARTYPE vt;
	hr = SafeArrayGetVartype(sarr_in, &vt);

	if (SUCCEEDED(hr))
	{
		// To make this code simple, we print only
		// SAFEARRAY(VT_I4)
		if (vt == VT_R8)
		{
			double* oVals;
			hr = SafeArrayAccessData(sarr_in, (void**)& oVals); // direct access to SA memory

			if (SUCCEEDED(hr))
			{
				long lowerBound, upperBound;  // get array bounds
				SafeArrayGetLBound(sarr_in, 1, &lowerBound);
				SafeArrayGetUBound(sarr_in, 1, &upperBound);

				long cnt_elements = upperBound - lowerBound + 1;

				for (int i = 0; i < cnt_elements; i++)  // iterate through returned values
				{
					double val = oVals[i];
					DPRINT(val);
				}

				SafeArrayUnaccessData(sarr_in);
			}
			else
			{
				// Error
			}
		}
	}
	
}

*/
