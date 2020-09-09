#pragma once
#include <vector>
#include <iostream>
#include <OAIdl.h>
#include "Beam.h"
//#include "OpticsElement.h"
#include "SceneElement.h"

#ifdef SCENE_EXPORTS
#define MY_API extern "C" __declspec(dllexport)
#else
#define MY_API extern "C" __declspec(dllimport)
#endif

#ifdef _DEBUG
#define DPRINT(x) std::cout << x << std::endl
#else
#define DPRINT(x)
#endif

#define PTR_ASSERT(x)	if ((x) == NULL) { DPRINT("Invalid Pointer! at: " << __FILE__ << "(" << __LINE__ << "): " << #x);}

#define MAX_BEAM_POINTS 20

struct IntersectionInformation
{
	SceneElement* element;
	double3 worldPoint;
	Ray* outRay;
};

class Scene
{
public:
	Scene();
	~Scene();
	/*
	static Scene& getInstance()
	{
		static Scene    instance; // Guaranteed to be destroyed.
							  // Instantiated on first use.
		DPRINT("Access to Scene: " << &instance);
		return instance;
	}*/

	Beam* CreateBeam(double3 position, double3 direction);
	SceneElement* CreateSceneElement(double3 position, double3 normal);
	void SetSceneElementTransform(SceneElement* element, double3 position, double3 normal);
	void GetBeamPoints(Beam* beam, std::vector<double3> &out_points);
	//void AddSceneElement()

	void DebugLog(const std::string str);
	void DebugLog(const std::string str, const double value);
	void DebugLog(const std::string str, const double3 value);

	IntersectionInformation NearestIntersection(Ray* ray);

	void TraceBeam(Beam* beam);

	
	//void AddSceneElement(ELEMENT_TYPE type, double3 position, double4 rotation);

private:
	/*
	Scene() : _beam {nullptr}
	{
		//For Debugging
			#ifdef _DEBUG
					FILE* pConsole;
					AllocConsole();
					freopen_s(&pConsole, "CONOUT$", "wb", stdout);
					DPRINT("************* START RUN **************");
					DPRINT("Created Scene: " << this);
					//DebugLog("Created Scene", )
			#endif // 
	}                    // Constructor? (the {} brackets) are needed here.
	*/
	Beam* _beam;
	//std::vector<Beam*> _beams;
	std::vector<SceneElement*> _sceneElements;

	/*
public:
	Scene(Scene const&) = delete;
	void operator=(Scene const&) = delete;W
	*/
};


MY_API Scene* ExtCreateNativeScene();
MY_API Beam* ExtCreateNativeBeam(Scene* scene, double position_arr_in[3], double direction_arr_in[3]);
MY_API SceneElement* ExtCreateNativeSceneElement(Scene* scene, double position_arr_in[3], double normal_arr_in[3]);

MY_API void ExtSetElementTransform(Scene* scene, SceneElement* element, double position_arr_in[3], double normal_arr_in[3]);

MY_API void ExtGetBeamPoints(Scene* scene, Beam* beam, double beam_points[3 * MAX_BEAM_POINTS]);

//MY_API void GetBeamPoints(Scene* scene, Beam* beam, SAFEARRAY*& sarr_in);

//MY_API void ExtOnUpdate();

